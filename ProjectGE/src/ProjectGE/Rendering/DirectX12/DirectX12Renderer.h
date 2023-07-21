#pragma once

#include "gepch.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include "ProjectGE/Rendering/Renderer.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"
#include "DirectXCommandQueue.h"
#include "DepthBuffer.h"
#include "BindableResources/BindableResource.h"

using Microsoft::WRL::ComPtr;

namespace ProjectGE {

	class DirectX12Renderer : public Renderer
	{
	public:
		DirectX12Renderer(HWND window);
		~DirectX12Renderer() = default;
		void Init() override;
		void Swap() override;
		void Resize(float width, float height) override;
		void SetClearColor(float r, float g, float b, float a) override;
		//void DrawDemoTriangle() override;

		inline ID3D12Device* GetDevice() { return m_Device.Get(); }
		inline DirectXCommandQueue* GetCommandQueue() { return m_Queue.get(); }

		inline ID3D12DescriptorHeap* GetSRVHeap() { return m_SRVHeapD.Get(); }
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTarget() const;
	private:
		void InitializeBackBuffer();
		
	
		// Debug Resources
		ComPtr<ID3D12Debug> m_Debug;
		ComPtr<ID3D12InfoQueue> m_InfoQueue;

		// Core Resources
		ComPtr<ID3D12Device8> m_Device;
		// This holds the two buffers we use for rendering (Presenting Buffer & Preparing Buffer)
		ComPtr<IDXGISwapChain4> m_SwapChain;

		// Graphics Handlers
		ComPtr<ID3D12Resource2> m_BackBuffer;
		// Render Target Heap
		ComPtr<ID3D12DescriptorHeap> m_RTVHeapD;
		ComPtr<ID3D12DescriptorHeap> m_SRVHeapD;
		// Depth Buffer
		std::unique_ptr<DepthBuffer> m_DBuffer;
		// The command queue for the renderer
		
		std::unique_ptr<DirectXCommandQueue> m_Queue;

		BOOL m_TearingSupport;
		static const int m_BufferCount = 2;

		HWND m_Window;
		
		bool m_DemoEnabled;
		float m_ClearColor[4] = { 0.07f, 0.0f, 0.12f, 1 };
	};

#define CREATE_DIRECTX12 new ProjectGE::DirectX
};

