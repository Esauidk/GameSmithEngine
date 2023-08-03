#pragma once

#include "gepch.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "ProjectGE/Rendering/RendererContext.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"
#include "CommandList/DirectX12CommandQueue.h"
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
		void SetClearColor(float r, float g, float b, float a) override;
		ComPtr<ID3D12GraphicsCommandList6> GetDrawCommandList(); // Get a direct type command list with Viewport, Rect, and RenderTarget set

		inline ID3D12DescriptorHeap* GetSRVHeap() { return m_SRVHeapD.Get(); }

		inline static ID3D12Device8* GetDevice() { return s_Device.Get(); }
		inline static ComPtr<ID3D12GraphicsCommandList6> GetDirectCommandList() { return s_DirectQueue->GetCommandList(); }
		inline static ComPtr<ID3D12GraphicsCommandList6> GetCopyCommandList() { return s_CopyQueue->GetCommandList(); }
		
		static UINT AsyncJobSubmission(ComPtr<ID3D12GraphicsCommandList6> jobList, D3D12_COMMAND_LIST_TYPE jobType);
		static void SyncJob(UINT fenceVal, D3D12_COMMAND_LIST_TYPE jobType);
		static void JobSubmission(ComPtr<ID3D12GraphicsCommandList6> jobList, D3D12_COMMAND_LIST_TYPE jobType);
	private:
		void InitializeBackBuffer();
		
	
		// Static members, needed for all instances
		static ComPtr<ID3D12Debug> s_Debug;
		static ComPtr<ID3D12InfoQueue> s_InfoQueue;
		static ComPtr<ID3D12Device8> s_Device;
		static std::unique_ptr<DirectX12CommandQueue> s_DirectQueue;
		static std::unique_ptr<DirectX12CommandQueue> s_CopyQueue;
		static bool s_Initialized;

		// Core Resources
		// This holds the two buffers we use for rendering (Presenting Buffer & Preparing Buffer)
		ComPtr<IDXGISwapChain4> m_SwapChain;
		// Graphics Handlers
		ComPtr<ID3D12Resource2> m_BackBuffer;
		// Render Target Heap
		ComPtr<ID3D12DescriptorHeap> m_RTVHeapD;
		ComPtr<ID3D12DescriptorHeap> m_SRVHeapD;
		// Depth Buffer
		std::unique_ptr<DirectX12DepthBuffer> m_DBuffer;
		

		BOOL m_TearingSupport;
		HWND m_Window;

		static const int m_BufferCount = 2;
		
		unsigned int m_Width;
		unsigned int m_Height;
		float m_ClearColor[4] = { 0.07f, 0.0f, 0.12f, 1 };
	};

};

