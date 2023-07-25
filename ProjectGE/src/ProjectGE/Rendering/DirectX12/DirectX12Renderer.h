#pragma once

#include "gepch.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "ProjectGE/Rendering/Renderer.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"
#include "DirectX12CommandQueue.h"
#include "DirectX12DepthBuffer.h"
#include "DirectX12TriangleDemo.h"


using Microsoft::WRL::ComPtr;

namespace ProjectGE {

	class DirectX12Renderer : public Renderer
	{
	public:
		DirectX12Renderer(HWND window, unsigned int initialWidth, unsigned int initialHeight);
		~DirectX12Renderer() = default;
		void Init() override;
		void Swap() override;
		void Resize(float width, float height) override;
		void SetClearColor(float r, float g, float b, float a) override;
		void SetDemoTriangle(bool enabled) override;

		inline ID3D12GraphicsCommandList6* GetDirectCommandList() { return m_DirectCmdList.Get(); }
		inline ID3D12GraphicsCommandList6* GetCopyCommandList() { return m_CopyCmdList.Get(); }

		inline DirectX12CommandQueue* GetCommandQueue() { return s_DirectQueue.get(); }

		inline ID3D12DescriptorHeap* GetSRVHeap() { return m_SRVHeapD.Get(); }
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTarget() const;

		inline static ID3D12Device8* GetDevice() { return s_Device.Get(); }
	private:
		void InitializeBackBuffer();
		void PreSwap();
		
	
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
		ComPtr<ID3D12GraphicsCommandList6> m_DirectCmdList;
		ComPtr<ID3D12GraphicsCommandList6> m_CopyCmdList;
		// Depth Buffer
		std::unique_ptr<DirectX12DepthBuffer> m_DBuffer;
		

		std::unique_ptr<DirectX12TriangleDemo> m_Demo;

		BOOL m_TearingSupport;
		HWND m_Window;

		static const int m_BufferCount = 2;
		
		unsigned int m_Width;
		unsigned int m_Height;
		bool m_DemoEnabled;
		float m_ClearColor[4] = { 0.07f, 0.0f, 0.12f, 1 };
	};

};

