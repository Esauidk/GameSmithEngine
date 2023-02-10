#pragma once
#include "Renderer.h"
#include "DirectXCommandQueue.h"
#include "BindableResource.h"
#include <vector>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <DirectXMath.h>

namespace Render {
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
	};

	class DirectXRenderer : public Renderer
	{
	public:
		DirectXRenderer(HWND hWnd);
		~DirectXRenderer() = default;
		bool Initialize(HWND hWnd) override;
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
		// Render Target View
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> pRTVHeapD;
		// Depth Stencil View
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> pDSVHeapD;

		// The command queue for the renderer
		DirectXCommandQueue queue;

		
		// Test Buffer
		BindableResource *buffer;
		BindableResource* index;
	};
};

