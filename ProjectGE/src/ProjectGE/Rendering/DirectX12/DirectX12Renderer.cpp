#include "gepch.h"
#include "DirectX12Renderer.h"

// Temporary Includes
#include "BindableResources/VertexBuffer.h"
#include "BindableResources/IndexBuffer.h"
#include "BindableResources/RootSignature.h"
#include "BindableResources/VertexShader.h"
#include "BindableResources/PixelShader.h"
#include "BindableResources/RootSignature.h"
#include "BindableResources/InputLayout.h"
#include "PipelineState.h"
#include "BindableResources/TopologyResource.h"
// END

#include <chrono>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

namespace ProjectGE {

	DirectX12Renderer::DirectX12Renderer(HWND window) : m_Window(window), m_Buffer(), m_DBuffer(), m_Index(), m_TearingSupport() {}

	void DirectX12Renderer::Init() {

		bool res;
		// Create the Debug Layer (Allows debuging on the device)
#if defined(GE_DEBUG)
		res = FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_Debug)));

		GE_CORE_ASSERT(!res, "Failed to create a DirectX12 debug interface");

		m_Debug->EnableDebugLayer();
#endif

		// Create Device
		res = FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_Device)));

		GE_CORE_ASSERT(!res, "Failed to create DirectX12 device");

#if defined(GE_DEBUG)
		// Create Info Queue, shows debug logs depending on severity and sets breaks
		res = FAILED(m_Device.As(&m_InfoQueue));
		GE_CORE_ASSERT(!res, "Failed to create DirectX12 info queue");

		m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);


		m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);


		m_InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
#endif

		m_Queue = DirectXCommandQueue(m_Device, D3D12_COMMAND_LIST_TYPE_DIRECT);

		/*// Gets our command queue, is used to send commands to GPU (draw calls, copy call, compute calls) [Lives on GPU]
		D3D12_COMMAND_QUEUE_DESC commandDesc = {};
		commandDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		commandDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		commandDesc.NodeMask = 0;

		RENDER_THROW(pDevice->CreateCommandQueue(&commandDesc, IID_PPV_ARGS(&pCommandQueue)));*/

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
		swapChainDesc.Width = 0;
		swapChainDesc.Height = 0;

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
			m_Queue.GetCommandQueue().Get(),
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

		res = FAILED(m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RTVHeapD)));
		GE_CORE_ASSERT(!res, "Failed to create DirectX12 render target");

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
		srvHeapDesc.NumDescriptors = 1;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		res = FAILED(m_Device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_SRVHeapD)));
		GE_CORE_ASSERT(!res, "Failed to create DirectX12 srv");

		UINT rtvSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		// Gets descriptor for render target view
		CD3DX12_CPU_DESCRIPTOR_HANDLE targetHandler = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart());
		Microsoft::WRL::ComPtr<ID3D12Resource2> buffer;

		// Assign target views to buffers and their place in the heap
		for (int i = 0; i < m_BufferCount; i++) {
			res = FAILED(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&buffer)));
			GE_CORE_ASSERT(!res, "Failed to create DirectX12 render target buffer");
			m_Device->CreateRenderTargetView(buffer.Get(), nullptr, targetHandler);
			targetHandler.Offset(rtvSize);
		}

		InitializeBackBuffer();

		m_DBuffer = new DepthBuffer(m_Device, 1080, 600);
	}

	void DirectX12Renderer::Swap()
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_BackBuffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT
		);
		ComPtr<ID3D12GraphicsCommandList6> list = m_Queue.GetCommandList();


		// Add command to transition the render target
		list->ResourceBarrier(1, &barrier);

		UINT value = m_Queue.ExecuteCommandList(list);
		m_Queue.WaitForFenceValue(value);


		bool res = FAILED(m_SwapChain->Present(0, 0));
		GE_CORE_ASSERT(!res, "Failed to present DirectX12 buffer");
		
		InitializeBackBuffer();
	}

	void DirectX12Renderer::Resize(float width, float height)
	{
		m_Queue.Flush();
		m_BackBuffer.Reset();

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		bool res = FAILED(m_SwapChain->GetDesc1(&swapChainDesc));
		GE_CORE_ASSERT(!res, "Failed to acquire back buffer description");

		res = FAILED(m_SwapChain->ResizeBuffers(swapChainDesc.BufferCount, (UINT)width, (UINT)height, swapChainDesc.Format, swapChainDesc.Flags));
		GE_CORE_ASSERT(!res, "Failed to resize DirectX12 swap chain buffers");

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart());
		UINT rtvSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		for (int i = 0; i < m_BufferCount; i++) {
			res = FAILED(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&m_BackBuffer)));
			GE_CORE_ASSERT(!res, "Failed to create DirectX12 render target buffer");
			m_Device->CreateRenderTargetView(m_BackBuffer.Get(), nullptr, rtv);
			rtv.Offset(rtvSize);
		}

		m_DBuffer->Resize(m_Device, (UINT)width, (UINT)height);

		InitializeBackBuffer();
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE DirectX12Renderer::GetRenderTarget() {
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart(), m_SwapChain->GetCurrentBackBufferIndex(), m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}

	void DirectX12Renderer::InitializeBackBuffer()
	{
		bool res = FAILED(m_SwapChain->GetBuffer(m_SwapChain->GetCurrentBackBufferIndex(), IID_PPV_ARGS(&m_BackBuffer)));
		GE_CORE_ASSERT(!res, "Failed to reacquire DirectX12 back buffer");

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			m_BackBuffer.Get(),
			D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_RENDER_TARGET
		);

		ComPtr<ID3D12GraphicsCommandList6> list = m_Queue.GetCommandList();
		// Add command to transition the render target
		list->ResourceBarrier(1, &barrier);
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart(), m_SwapChain->GetCurrentBackBufferIndex(), m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		const float color[] = { 0.07f, 0.0f, 0.12f, 1 };
		list->ClearRenderTargetView(rtv, color, 0, nullptr);
		UINT value = m_Queue.ExecuteCommandList(list);
		m_Queue.WaitForFenceValue(value);
	}

	void DirectX12Renderer::CreateObject() {
		// Shader Test!
		VertexShader vs("ExampleVertexShader.cso");
		PixelShader ps("PixelShader.cso");

		// Input Layout Test!
		D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		InputLayout layout(inputLayout, 2);

		// Create RootSignature
		RootSignature root(m_Device, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS);

		CD3DX12_ROOT_PARAMETER1 rootParameters;
		rootParameters.InitAsConstants(sizeof(DirectX::XMMATRIX) / 4, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
		root.AddParameter(rootParameters);

		root.BuildRootSignature(m_Device);

		D3D12_RT_FORMAT_ARRAY rtvFormats = {};
		rtvFormats.NumRenderTargets = 1;
		rtvFormats.RTFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

		TopologyResource top(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);




		// Pipeline State Object Test : Define Pipeline and bind some bindables


		PipelineState state;
		vs.Setup(state);
		ps.Setup(state);
		layout.Setup(state);
		root.Setup(state);
		top.Setup(state);
		state.Attach(rtvFormats);
		state.Attach(DXGI_FORMAT_D32_FLOAT);
		state.Build(m_Device);

		ComPtr<ID3D12GraphicsCommandList6> list = m_Queue.GetCommandList();
		using DirectX::XMFLOAT3;

		Vertex cubeVertex[] = {
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f) }, // 0
			{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) }, // 1
			{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f) }, // 2
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) }, // 3
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) }, // 4
			{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT3(0.0f, 1.0f, 1.0f) }, // 5
			{ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f) }, // 6
			{ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(1.0f, 0.0f, 1.0f) }
		};

		VertexBuffer<Vertex> tempBuffer(m_Device, list, cubeVertex, sizeof(cubeVertex) / sizeof(Vertex));

		WORD indexCount[] = {
			0, 1, 2, 0, 2, 3,
			4, 6, 5, 4, 7, 6,
			4, 5, 1, 4, 1, 0,
			3, 2, 6, 3, 6, 7,
			1, 5, 6, 1, 6, 2,
			4, 0, 3, 4, 3, 7
		};



		IndexBuffer tempIndex = IndexBuffer(m_Device, list, indexCount, _countof(indexCount));
		UINT value = m_Queue.ExecuteCommandList(list);
		m_Queue.WaitForFenceValue(value);

		list = m_Queue.GetCommandList();

		state.Bind(list);
		root.Bind(list);
		top.Bind(list);
		tempBuffer.Bind(list);
		tempIndex.Bind(list);

		CD3DX12_RECT rect = CD3DX12_RECT(0, 0, LONG_MAX, LONG_MAX);
		CD3DX12_VIEWPORT viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, 1080, 600);

		list->RSSetScissorRects(1, &rect);
		list->RSSetViewports(1, &viewport);
		CD3DX12_CPU_DESCRIPTOR_HANDLE targetHandler(m_RTVHeapD->GetCPUDescriptorHandleForHeapStart(), m_SwapChain->GetCurrentBackBufferIndex(), m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		D3D12_CPU_DESCRIPTOR_HANDLE depthHandler = m_DBuffer->GetHandle();
		list->OMSetRenderTargets(1, &targetHandler, FALSE, &depthHandler);

		float angle = static_cast<float>(90.0);
		const DirectX::XMVECTOR rotationAxis = DirectX::XMVectorSet(0, 1, 1, 0);
		DirectX::XMMATRIX m = DirectX::XMMatrixRotationAxis(rotationAxis, DirectX::XMConvertToRadians(angle));

		const DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0, 0, -10, 1);
		const DirectX::XMVECTOR focusPoint = DirectX::XMVectorSet(0, 0, 0, 1);
		const DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0, 1, 0, 0);
		DirectX::XMMATRIX v = DirectX::XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);

		DirectX::XMMATRIX p = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), 1080 / 600, 0.1f, 100.0f);

		DirectX::XMMATRIX mvpMatrix = DirectX::XMMatrixMultiply(m, v);
		mvpMatrix = XMMatrixMultiply(mvpMatrix, p);

		list->SetGraphicsRoot32BitConstants(0, sizeof(DirectX::XMMATRIX) / 4, &mvpMatrix, 0);

		list->DrawIndexedInstanced(_countof(indexCount), 1, 0, 0, 0);

		value = m_Queue.ExecuteCommandList(list);
		m_Queue.WaitForFenceValue(value);

	}

	void DirectX12Renderer::DrawObject() {
		CreateObject();
	}
};


