#include "gepch.h"
#include "DirectX12Renderer.h"
#include "ProjectGE/Log.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

namespace ProjectGE {
	ComPtr<ID3D12Debug> DirectX12Renderer::s_Debug = nullptr;
	ComPtr<ID3D12InfoQueue> DirectX12Renderer::s_InfoQueue = nullptr;
	ComPtr<ID3D12Device8> DirectX12Renderer::s_Device = nullptr;
	std::unique_ptr<DirectX12CommandQueue> DirectX12Renderer::s_DirectQueue = nullptr;
	std::unique_ptr<DirectX12CommandQueue> DirectX12Renderer::s_CopyQueue = nullptr;
	bool DirectX12Renderer::s_Initialized = false;


	DirectX12Renderer::DirectX12Renderer(HWND window, unsigned int initialWidth, unsigned int initialHeight) : m_DBuffer(), m_TearingSupport(), m_Window(window), m_Width(initialWidth), m_Height(initialHeight) {}

	void DirectX12Renderer::Init() {

		bool res;
		// Create the Debug Layer (Allows debuging on the device)
#if defined(GE_DEBUG)
		if (!s_Initialized) {
			res = FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&s_Debug)));

			GE_CORE_ASSERT(!res, "Failed to create a DirectX12 debug interface");

			s_Debug->EnableDebugLayer();
		}
		
#endif
		if (!s_Initialized) {
			// Create Device
			res = FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&s_Device)));

			GE_CORE_ASSERT(!res, "Failed to create DirectX12 device");
		}
		

#if defined(GE_DEBUG)

		if (!s_Initialized) {
			// Create Info Queue, shows debug logs depending on severity and sets breaks
			res = FAILED(s_Device.As(&s_InfoQueue));
			GE_CORE_ASSERT(!res, "Failed to create DirectX12 info queue");

			s_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);


			s_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);


			s_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
		}
		
#endif
		if (!s_Initialized) {
			s_DirectQueue = std::make_unique<DirectX12CommandQueue>(s_Device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
			s_CopyQueue = std::make_unique<DirectX12CommandQueue>(s_Device.Get(), D3D12_COMMAND_LIST_TYPE_COPY);
		}
		
		s_Initialized = true;

		/**********************************************/
		// SWAP-CHAIN CREATION
		/*********************************************/

		ComPtr<IDXGIFactory5> dxgiFactory5;

		UINT createFactoryFlags = 0;

#if defined(GE_DEBUG)
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
		res = FAILED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory5)));
		GE_CORE_ASSERT(!res, "Failed to create DirectX12 DXGI factory");

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

		// Make them native size for now (potentially allow resolution changing in the future)
		swapChainDesc.Width = m_Width;
		swapChainDesc.Height = m_Height;

		// 8 Bytes per channel, unormalized
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// Full-screen mode?
		swapChainDesc.Stereo = FALSE;
		// One Sampling, Normal Quality
		swapChainDesc.SampleDesc = { 1,0 };
		// Buffer used for rending to output (i.e. monitor/application)
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		// The amount fo buffers in the swap chain
		swapChainDesc.BufferCount = m_BufferCount;
		// Stretch if buffer doesn't fit resolution
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		// Discard contents of back buffer after presentation call (Fresh new buffer after each presentation call)
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		// Nothing really done with alphas
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

		m_TearingSupport = FALSE;
		if (FAILED(dxgiFactory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING,
			&m_TearingSupport,
			sizeof(m_TearingSupport)))) {
			m_TearingSupport = FALSE;
		}

		swapChainDesc.Flags = (m_TearingSupport == TRUE) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

		ComPtr<IDXGISwapChain1> dxgiSwapChain1;

		// Create OS tied Swap-Chain
		res = FAILED(dxgiFactory5->CreateSwapChainForHwnd(
			s_DirectQueue->GetCommandQueue(),
			m_Window,
			&swapChainDesc,
			nullptr,
			nullptr,
			&dxgiSwapChain1));

		GE_CORE_ASSERT(!res, "Failed to create DirectX12 swap chain");

		// Disables auto-fullscreen (Alt-Enter)
		res = FAILED(dxgiFactory5->MakeWindowAssociation(m_Window, DXGI_MWA_NO_ALT_ENTER));
		GE_CORE_ASSERT(!res, "Failed to diable auto-fullscreen");

		// Upgrades swap chan to SwapChain4
		res = FAILED(dxgiSwapChain1.As(&m_SwapChain));
		GE_CORE_ASSERT(!res, "Failed to cast DirectX12 swap chain");

		// Describe a heap of descriptors (Need one for each back buffer since they will have their own target render views)
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = 2;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

		res = FAILED(s_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RTVHeapD)));
		GE_CORE_ASSERT(!res, "Failed to create DirectX12 render target");

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		res = FAILED(s_Device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_SRVHeapD)));
		GE_CORE_ASSERT(!res, "Failed to create DirectX12 srv");

		UINT rtvSize = s_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// Gets descriptor for render target view
		CD3DX12_CPU_DESCRIPTOR_HANDLE targetHandler = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart());
		Microsoft::WRL::ComPtr<ID3D12Resource2> buffer;

		// Assign target views to buffers and their place in the heap
		for (int i = 0; i < m_BufferCount; i++) {
			res = FAILED(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&buffer)));
			GE_CORE_ASSERT(!res, "Failed to create DirectX12 render target buffer");
			s_Device->CreateRenderTargetView(buffer.Get(), nullptr, targetHandler);
			targetHandler.Offset(rtvSize);
		}

		m_DBuffer = std::make_unique<DirectX12DepthBuffer>(s_Device.Get(), m_Width, m_Height);

		InitializeBackBuffer();

		
	}

	void DirectX12Renderer::Swap()
	{
		PreSwap();

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_BackBuffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT
		);


		// Add command to transition the render target
		m_DirectCmdList->ResourceBarrier(1, &barrier);

		UINT value = s_DirectQueue->ExecuteCommandList(m_DirectCmdList);
		s_DirectQueue->WaitForFenceValue(value);


		bool res = FAILED(m_SwapChain->Present(0, 0));
		GE_CORE_ASSERT(!res, "Failed to present DirectX12 buffer");
		
		InitializeBackBuffer();
	}

	void DirectX12Renderer::Resize(float width, float height)
	{
		m_Width = (UINT)width;
		m_Height = (UINT)height;

		s_DirectQueue->Flush();
		m_BackBuffer.Reset();

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		bool res = FAILED(m_SwapChain->GetDesc1(&swapChainDesc));
		GE_CORE_ASSERT(!res, "Failed to acquire back buffer description");

		res = FAILED(m_SwapChain->ResizeBuffers(swapChainDesc.BufferCount, m_Width, m_Height, swapChainDesc.Format, swapChainDesc.Flags));
		GE_CORE_ASSERT(!res, "Failed to resize DirectX12 swap chain buffers");

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart());
		UINT rtvSize = s_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		for (int i = 0; i < m_BufferCount; i++) {
			res = FAILED(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_BackBuffer)));
			GE_CORE_ASSERT(!res, "Failed to create DirectX12 render target buffer");
			s_Device->CreateRenderTargetView(m_BackBuffer.Get(), nullptr, rtv);
			rtv.Offset(rtvSize);
		}

		m_DBuffer->Resize(s_Device.Get(), m_Width, m_Height);

		InitializeBackBuffer();
	}

	void DirectX12Renderer::SetClearColor(float r, float g, float b, float a)
	{
		m_ClearColor[0] = r;
		m_ClearColor[1] = g;
		m_ClearColor[2] = b;
		m_ClearColor[3] = a;
	}

	void DirectX12Renderer::SetDemoTriangle(bool enabled)
	{
		m_DemoEnabled = enabled;

		if (m_DemoEnabled) {
			m_Demo = std::make_unique<DirectX12TriangleDemo>(s_Device.Get(), m_CopyCmdList.Get());
			UINT val = s_CopyQueue->ExecuteCommandList(m_CopyCmdList);
			s_CopyQueue->WaitForFenceValue(val);
			m_CopyCmdList = s_CopyQueue->GetCommandList();
		}
		else {
			m_Demo.reset();
		}
	}


	CD3DX12_CPU_DESCRIPTOR_HANDLE DirectX12Renderer::GetRenderTarget() const {
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart(), m_SwapChain->GetCurrentBackBufferIndex(), s_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}

	void DirectX12Renderer::InitializeBackBuffer()
	{
		m_DirectCmdList = s_DirectQueue->GetCommandList();
		m_CopyCmdList = s_CopyQueue->GetCommandList();

		bool res = FAILED(m_SwapChain->GetBuffer(m_SwapChain->GetCurrentBackBufferIndex(), IID_PPV_ARGS(&m_BackBuffer)));
		GE_CORE_ASSERT(!res, "Failed to reacquire DirectX12 back buffer");

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_BackBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET
		);
		// Add command to transition the render target
		m_DirectCmdList->ResourceBarrier(1, &barrier);
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart(), m_SwapChain->GetCurrentBackBufferIndex(), s_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		m_DirectCmdList->ClearRenderTargetView(rtv, m_ClearColor, 0, nullptr);
		m_DBuffer->Clear(m_DirectCmdList.Get(), 1);

		auto rect = CD3DX12_RECT(0, 0, m_Width, m_Height);
		auto viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, (FLOAT)m_Width, (FLOAT)m_Height);
		m_DirectCmdList->RSSetScissorRects(1, &rect);
		m_DirectCmdList->RSSetViewports(1, &viewport);

		D3D12_CPU_DESCRIPTOR_HANDLE depthHandler = m_DBuffer->GetHandle();
		m_DirectCmdList->OMSetRenderTargets(1, &rtv, FALSE, &depthHandler);
	}

	void DirectX12Renderer::PreSwap()
	{
		if (m_DemoEnabled) {
			m_Demo->Draw(m_DirectCmdList.Get());
		}
	}
};


