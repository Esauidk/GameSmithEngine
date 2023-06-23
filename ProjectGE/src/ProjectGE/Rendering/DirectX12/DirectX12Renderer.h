#pragma once

#include "gepch.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>
#include "ProjectGE/Rendering/Renderer.h"
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
	private:
		// Debug Resources
		Microsoft::WRL::ComPtr<ID3D12Debug> pDebug;
		Microsoft::WRL::ComPtr<ID3D12InfoQueue> pInfoQueue;

		// Core Resources
		Microsoft::WRL::ComPtr<ID3D12Device8> pDevice;
		// This holds the two buffers we use for rendering (Presenting Buffer & Preparing Buffer)
		Microsoft::WRL::ComPtr<IDXGISwapChain4> pSwapChain;

		// Graphics Handlers
		Microsoft::WRL::ComPtr<ID3D12Resource2> pBackBuffer;
		// Render Target Heap
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> pRTVHeapD;
		// Depth Buffer
		DepthBuffer* dBuffer;
		// The command queue for the renderer
		DirectXCommandQueue queue;

		HWND m_Window;
		// Test Buffer
		BindableResource *buffer;
		BindableResource* index;
	};

#define CREATE_DIRECTX12 new ProjectGE::DirectX
};

