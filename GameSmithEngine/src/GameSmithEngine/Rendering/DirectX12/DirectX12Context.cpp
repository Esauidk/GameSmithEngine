#include "gepch.h"
#include "DirectX12Context.h"
#include "GameSmithEngine/Core/Log.h"

#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12Views.h"
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12Resource.h"
#include "GameSmithEngine/Rendering/DirectX12/RenderComponents/Texture/DirectX12RenderTexture.h"

#include "GameSmithEngine/Rendering/RenderingManager.h"

namespace GameSmith {

	DirectX12Context::DirectX12Context(HWND window, unsigned int initialWidth, unsigned int initialHeight) : m_DBuffer(), m_TearingSupport(), m_Window(window), m_FrameWidth(initialWidth), m_FrameHeight(initialHeight) {}

	void DirectX12Context::Init() {

		/**********************************************/
		// SWAP-CHAIN CREATION
		/*********************************************/

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

		// Intialize width & height
		swapChainDesc.Width = m_FrameWidth;
		swapChainDesc.Height = m_FrameHeight;

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
		swapChainDesc.Scaling = DXGI_SCALING_NONE;
		// Discard contents of back buffer after presentation call (Fresh new buffer after each presentation call)
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		// Nothing really done with alphas
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

		m_TearingSupport = FALSE;

		IDXGIFactory5* factory = DirectX12Core::GetCore().GetFactory();

		if (FAILED(factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING,
			&m_TearingSupport,
			sizeof(m_TearingSupport)))) {
			m_TearingSupport = FALSE;
		}

		swapChainDesc.Flags = (m_TearingSupport == TRUE) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;


		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();

		ComPtr<IDXGISwapChain1> dxgiSwapChain1;
		// Create OS tied Swap-Chain
		bool res = FAILED(factory->CreateSwapChainForHwnd(
			core.GetDirectCommandContext()->GetQueue().GetCommandQueue(),
			m_Window,
			&swapChainDesc,
			nullptr,
			nullptr,
			&dxgiSwapChain1));

		GE_CORE_ASSERT(!res, "Failed to create DirectX12 swap chain");

		// Disables auto-fullscreen (Alt-Enter)
		res = FAILED(factory->MakeWindowAssociation(m_Window, DXGI_MWA_NO_ALT_ENTER));
		GE_CORE_ASSERT(!res, "Failed to diable auto-fullscreen");

		// Upgrades swap chan to SwapChain4
		res = FAILED(dxgiSwapChain1.As(&m_SwapChain));
		GE_CORE_ASSERT(!res, "Failed to cast DirectX12 swap chain");

		// Gets descriptor for render target view
		Microsoft::WRL::ComPtr<ID3D12Resource2> buffer;

		auto& descriptorLoader = core.GetDescriptorLoader(RT);
		// Assign target views to buffers and their place in the heap
		for (int i = 0; i < m_BufferCount; i++) {
			res = FAILED(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&buffer)));
			GE_CORE_ASSERT(!res, "Failed to create DirectX12 render target buffer");
			m_RTV[i].m_View = descriptorLoader.AllocateSlot();
			m_RTV[i].m_Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			device->CreateRenderTargetView(buffer.Get(), nullptr, m_RTV[i].m_View);
		}

		m_DBuffer = Scope<DirectX12DepthBuffer>(new DirectX12DepthBuffer(device, m_FrameWidth, m_FrameHeight));

		InitializeBackBuffer();

		auto rect = CD3DX12_RECT(0, 0, m_FrameWidth, m_FrameHeight);
		auto viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, (FLOAT)m_FrameWidth, (FLOAT)m_FrameHeight);
		auto& state = core.GetDirectCommandContext()->GetStateManager();

		state.SetRects(&rect, 1);
		state.SetViewports(&viewport, 1);
		
	}

	bool DirectX12Context::Swap()
	{
		auto& core = DirectX12Core::GetCore();
		auto context = core.GetDirectCommandContext();
		
		if (core.FrameReady()) {
			core.SwappingFrame();
		}
		else {
			// TODO: Add logic for if we're under an FPS constraint
			core.SwappingFrame();
		}

		auto frameTex = RenderingManager::GetInstance()->GetTextureForFrame();
		if (frameTex != nullptr) {
			auto d3Frame = CastPtr<DirectX12RenderTexture>(frameTex);
			d3Frame->CopyToResource(m_BackBuffer, context);
		}
		
		m_BackBuffer->GetResourceStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_PRESENT, context);
		context->FinalizeCommandList();
		context->SubmitCommandLists();
		core.InitializeCPUQueueWait(DirectX12QueueType::Direct);

		bool res;
		if (m_Vsync) {
			res = FAILED(m_SwapChain->Present(1, 0));
		}
		else {
			res = FAILED(m_SwapChain->Present(0, 0));
		}
		GE_CORE_ASSERT(!res, "Failed to present DirectX12 buffer");
		core.GetHeapDatabase()->FrameEnded();

		InitializeBackBuffer();

		return true;
	}

	void DirectX12Context::Resize(float width, float height, bool useForRender)
	{
		m_FrameWidth = (UINT)width;
		m_FrameHeight = (UINT)height;

		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();
		auto context = core.GetDirectCommandContext();
		context->FinalizeCommandList();
		context->SubmitCommandLists();
		context->GetQueue().Flush();
		m_BackBuffer.reset();

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		bool res = FAILED(m_SwapChain->GetDesc1(&swapChainDesc));
		GE_CORE_ASSERT(!res, "Failed to acquire back buffer description");

		res = FAILED(m_SwapChain->ResizeBuffers(swapChainDesc.BufferCount, m_FrameWidth, m_FrameHeight, swapChainDesc.Format, swapChainDesc.Flags));
		GE_CORE_ASSERT(!res, "Failed to resize DirectX12 swap chain buffers");

		// Gets descriptor for render target view
		Microsoft::WRL::ComPtr<ID3D12Resource2> buffer;
		for (int i = 0; i < m_BufferCount; i++) {
			res = FAILED(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&buffer)));
			GE_CORE_ASSERT(!res, "Failed to create DirectX12 render target buffer");
			device->CreateRenderTargetView(buffer.Get(), nullptr, m_RTV[i].m_View);
		}

		m_DBuffer->Resize(device, m_FrameWidth, m_FrameHeight);

		if (useForRender) {
			ResizeRenderSpace(0, 0, width, height);
		}
		

		InitializeBackBuffer();
	}

	void DirectX12Context::ResizeRenderSpace(float posX, float posY, float width, float height)
	{
		m_RenderWidth = (UINT)width;
		m_RenderHeight = (UINT)height;
		m_RenderPosX = (UINT)posX;
		m_RenderPosY = (UINT)posY;

		auto rect = CD3DX12_RECT(m_RenderPosX, m_RenderPosY, m_RenderWidth, m_RenderHeight);
		auto viewport = CD3DX12_VIEWPORT((FLOAT)m_RenderPosX, (FLOAT)m_RenderPosY, (FLOAT)m_RenderWidth, (FLOAT)m_RenderHeight);
		auto& state = DirectX12Core::GetCore().GetDirectCommandContext()->GetStateManager();

		state.SetRects(&rect, 1);
		state.SetViewports(&viewport, 1);
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


	void DirectX12Context::InitializeBackBuffer()
	{
		auto& core = DirectX12Core::GetCore();
		auto device = core.GetDevice();
		auto context = core.GetDirectCommandContext();
		auto& cmdList = context->GetCommandList();

		m_CurrentBackBuffer = m_SwapChain->GetCurrentBackBufferIndex();

		ComPtr<ID3D12Resource2> backBuffer;
		bool res = FAILED(m_SwapChain->GetBuffer(m_CurrentBackBuffer, IID_PPV_ARGS(&backBuffer)));
		GE_CORE_ASSERT(!res, "Failed to reacquire DirectX12 back buffer");

		m_BackBuffer = Ref<DirectX12Resource>(new DirectX12Resource(backBuffer, D3D12_RESOURCE_STATE_COMMON));
		
		m_BackBuffer->GetResourceStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_COPY_DEST, context);
		context->FinalizeResourceBarriers();

		m_DBuffer->Clear(&cmdList, 1);
		// TODO: from an ownership prespective having the context own clearing the textures, doesn't make sense. THis might be better as an event
		RenderingManager::GetInstance()->ClearTextures();

		context->FinalizeCommandList();
		context->RequestWait(DirectX12QueueType::Direct);
		context->SubmitCommandLists();


		D3D12_CPU_DESCRIPTOR_HANDLE depthHandler = m_DBuffer->GetHandle();
		DirectX12DepthTargetView depthView;
		depthView.m_View = depthHandler;

		context->GetStateManager().SetDepthTarget(depthView);

	}


	

};


