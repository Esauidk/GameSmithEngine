#pragma once

#include "gepch.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "ProjectGE/Rendering/RendererContext.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"
#include "CommandList/DirectX12CommandContext.h"
#include "DirectX12DepthBuffer.h"


using Microsoft::WRL::ComPtr;

namespace ProjectGE {

	class DirectX12Context : public RendererContext
	{
	public:
		DirectX12Context(HWND window, unsigned int initialWidth, unsigned int initialHeight);
		~DirectX12Context() = default;
		void Init() override;
		void Swap() override;
		void Resize(float width, float height) override;
		void SetVsync(bool vsync) override;
		void SetClearColor(float r, float g, float b, float a) override;
		void AttachContextResources() override; // Attach the Viewport, Rect, and RenderTarget set

		inline ID3D12DescriptorHeap* GetSRVHeap() { return m_SRVHeapD.Get(); }

		inline static ID3D12Device8* GetDevice() { return s_Device.Get(); }
		inline static DirectX12CommandListWrapper& GetDirectCommandList() { return s_DirectContext->GetCommandList(); }
		inline static DirectX12CommandListWrapper& GetCopyCommandList() { return s_CopyContext->GetCommandList(); }
		
		// Finish recording on a type of command list
		static UINT FinalizeCommandList(DirectX12QueueType type);
		// Tell one queue to wait for another queue to complete a certain amount of work
		static void InitializeQueueWait(DirectX12QueueType executor, DirectX12QueueType waiter, UINT fenceVal);
		// Tell the CPU to wait for all work to complete on a specific queue
		static void InitializeCPUQueueWait(DirectX12QueueType target);
		// Tell the CPU to wait for some work to be complete on a specific queue
		static void InitializeCPUQueueWait(UINT fenceVal, DirectX12QueueType target);
	private:
		void InitializeBackBuffer();
		static DirectX12CommandQueue& FindQueue(DirectX12QueueType type);
	
		// Static members, needed for all instances
		static ComPtr<ID3D12Debug> s_Debug;
		static ComPtr<ID3D12InfoQueue> s_InfoQueue;
		static ComPtr<ID3D12Device8> s_Device;
		static std::unique_ptr<DirectX12CommandContextDirect> s_DirectContext;
		static std::unique_ptr<DirectX12CommandContextCopy> s_CopyContext;
		static bool s_Initialized;

		// Core Resources
		// This holds the two buffers we use for rendering (Presenting Buffer & Preparing Buffer)
		ComPtr<IDXGISwapChain4> m_SwapChain;
		// Graphics Handlers
		ComPtr<ID3D12Resource2> m_BackBuffer;
		// Render Target Heap
		ComPtr<ID3D12DescriptorHeap> m_RTVHeapD;
		// Shader Resource Heap
		ComPtr<ID3D12DescriptorHeap> m_SRVHeapD;
		// Depth Buffer
		std::unique_ptr<DirectX12DepthBuffer> m_DBuffer;
		
		bool m_Vsync;
		BOOL m_TearingSupport;
		HWND m_Window;

		static const int m_BufferCount = 2;
		
		unsigned int m_Width;
		unsigned int m_Height;
		float m_ClearColor[4] = { 0.07f, 0.0f, 0.12f, 1 };
	};

};

