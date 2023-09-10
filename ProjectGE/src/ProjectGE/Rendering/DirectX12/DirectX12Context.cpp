#include "gepch.h"
#include "DirectX12Context.h"
#include "ProjectGE/Core/Log.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12Views.h"

namespace ProjectGE {

	DirectX12Context::DirectX12Context(HWND window, unsigned int initialWidth, unsigned int initialHeight) : m_DBuffer(), m_TearingSupport(), m_Window(window), m_Width(initialWidth), m_Height(initialHeight) {}

	void DirectX12Context::Init() {

		/**********************************************/
		// SWAP-CHAIN CREATION
		/*********************************************/

		ComPtr<IDXGIFactory5> dxgiFactory5;

		UINT createFactoryFlags = 0;

#if defined(GE_DEBUG)
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
		bool res = FAILED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory5)));
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


		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		ComPtr<IDXGISwapChain1> dxgiSwapChain1;
		// Create OS tied Swap-Chain
		res = FAILED(dxgiFactory5->CreateSwapChainForHwnd(
			core.GetDirectCommandContext().GetQueue().GetCommandQueue(),
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

		res = FAILED(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RTVHeapD)));
		GE_CORE_ASSERT(!res, "Failed to create DirectX12 render target");


		UINT rtvSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// Gets descriptor for render target view
		CD3DX12_CPU_DESCRIPTOR_HANDLE targetHandler = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart());
		Microsoft::WRL::ComPtr<ID3D12Resource2> buffer;

		// Assign target views to buffers and their place in the heap
		for (int i = 0; i < m_BufferCount; i++) {
			res = FAILED(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&buffer)));
			GE_CORE_ASSERT(!res, "Failed to create DirectX12 render target buffer");
			device->CreateRenderTargetView(buffer.Get(), nullptr, targetHandler);
			targetHandler.Offset(rtvSize);
		}

		m_DBuffer = std::make_unique<DirectX12DepthBuffer>(device, m_Width, m_Height);

		InitializeBackBuffer();

		
	}

	void DirectX12Context::Swap()
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_BackBuffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT
		);

		auto& core = DirectX12Core::GetCore();
		auto& context = core.GetDirectCommandContext();
		auto& list = context.GetCommandList();
		// Add command to transition the render target
		list->ResourceBarrier(1, &barrier);

		
		UINT val = context.FinalizeCommandList();
		core.InitializeCPUQueueWait(DirectX12QueueType::Direct);

		bool res;
		if (m_Vsync) {
			res = FAILED(m_SwapChain->Present(1, 0));
		}
		else {
			res = FAILED(m_SwapChain->Present(0, 0));
		}
		GE_CORE_ASSERT(!res, "Failed to present DirectX12 buffer");
		
		InitializeBackBuffer();
	}

	void DirectX12Context::Resize(float width, float height)
	{
		m_Width = (UINT)width;
		m_Height = (UINT)height;

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();
		auto& context = core.GetDirectCommandContext();
		context.FinalizeCommandList();
		context.GetQueue().Flush();
		m_BackBuffer.Reset();

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		bool res = FAILED(m_SwapChain->GetDesc1(&swapChainDesc));
		GE_CORE_ASSERT(!res, "Failed to acquire back buffer description");

		res = FAILED(m_SwapChain->ResizeBuffers(swapChainDesc.BufferCount, m_Width, m_Height, swapChainDesc.Format, swapChainDesc.Flags));
		GE_CORE_ASSERT(!res, "Failed to resize DirectX12 swap chain buffers");

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart());
		UINT rtvSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		for (int i = 0; i < m_BufferCount; i++) {
			res = FAILED(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_BackBuffer)));
			GE_CORE_ASSERT(!res, "Failed to create DirectX12 render target buffer");
			device->CreateRenderTargetView(m_BackBuffer.Get(), nullptr, rtv);
			rtv.Offset(rtvSize);
		}

		m_DBuffer->Resize(device, m_Width, m_Height);

		// NOTE: THIS MIGHT CAUSE A BUG. If there is an active commandlist in the command queue when resizing, dx12 will complain
		//s_DirectContext->StartCommandList();
		InitializeBackBuffer();
	}

	void DirectX12Context::SetVsync(bool vsync)
	{
		m_Vsync = vsync;
	}

	void DirectX12Context::SetClearColor(float r, float g, float b, float a)
	{
		m_ClearColor[0] = r;
		m_ClearColor[1] = g;
		m_ClearColor[2] = b;
		m_ClearColor[3] = a;
	}

	void DirectX12Context::AttachContextResources()
	{
		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();
		auto& context = core.GetDirectCommandContext();
		auto& cmdList = context.GetCommandList();

		auto rect = CD3DX12_RECT(0, 0, m_Width, m_Height);
		auto viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, (FLOAT)m_Width, (FLOAT)m_Height);
		cmdList->RSSetScissorRects(1, &rect);
		cmdList->RSSetViewports(1, &viewport);

		D3D12_CPU_DESCRIPTOR_HANDLE depthHandler = m_DBuffer->GetHandle();
		DirectX12DepthTargetView depthView;
		depthView.m_View = depthHandler;
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart(), m_SwapChain->GetCurrentBackBufferIndex(), device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		m_RTVs = std::make_shared<DirectX12RenderTargetView>();
		m_RTVs->m_View = rtv;
		m_RTVs->m_Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		core.GetDirectCommandContext().GetStateManager().SetRenderTarget(m_RTVs, 1, depthView);
		//cmdList->OMSetRenderTargets(1, &rtv, FALSE, &depthHandler);
	}


	void DirectX12Context::InitializeBackBuffer()
	{
		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();
		auto& context = core.GetDirectCommandContext();
		auto& cmdList = context.GetCommandList();

		bool res = FAILED(m_SwapChain->GetBuffer(m_SwapChain->GetCurrentBackBufferIndex(), IID_PPV_ARGS(&m_BackBuffer)));
		GE_CORE_ASSERT(!res, "Failed to reacquire DirectX12 back buffer");

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_BackBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET
		);
		// Add command to transition the render target
		cmdList->ResourceBarrier(1, &barrier);
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart(), m_SwapChain->GetCurrentBackBufferIndex(), device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		cmdList->ClearRenderTargetView(rtv, m_ClearColor, 0, nullptr);
		m_DBuffer->Clear(&cmdList, 1);

	}


	

};


