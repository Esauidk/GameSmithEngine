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

namespace ProjectGE {
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
	};

	class DirectX12Renderer : public Renderer
	{
	public:
		DirectX12Renderer(HWND window);
		~DirectX12Renderer() = default;
		void Init() override;
		void StartFrame() override;
		void EndFrame() override;
		void CreateObject() override;
		void DrawObject() override;

		inline Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return m_Device; }
		inline DirectXCommandQueue& GetCommandQueue() { return m_Queue; }

		inline Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> GetSRVHeap() { return m_SRVHeapD; }
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTarget();
	private:
		// Debug Resources
		Microsoft::WRL::ComPtr<ID3D12Debug> m_Debug;
		Microsoft::WRL::ComPtr<ID3D12InfoQueue> m_InfoQueue;

		// Core Resources
		Microsoft::WRL::ComPtr<ID3D12Device8> m_Device;
		// This holds the two buffers we use for rendering (Presenting Buffer & Preparing Buffer)
		Microsoft::WRL::ComPtr<IDXGISwapChain4> m_SwapChain;

		// Graphics Handlers
		Microsoft::WRL::ComPtr<ID3D12Resource2> m_BackBuffer;
		// Render Target Heap
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RTVHeapD;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_SRVHeapD;
		// Depth Buffer
		DepthBuffer* m_DBuffer;
		// The command queue for the renderer
		DirectXCommandQueue m_Queue;

		HWND m_Window;
		// Test Buffer
		BindableResource* m_Buffer;
		BindableResource* m_Index;
	};

#define CREATE_DIRECTX12 new ProjectGE::DirectX
};

