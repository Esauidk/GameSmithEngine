#pragma once

#include "gepch.h"
#include "ProjectGE/Rendering/RendererContext.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12DepthBuffer.h"
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12Views.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "ProjectGE/Core/Core.h"

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

	private:
		void InitializeBackBuffer();

		// Core Resources
		// This holds the two buffers we use for rendering (Presenting Buffer & Preparing Buffer)
		ComPtr<IDXGISwapChain4> m_SwapChain;
		// Graphics Handlers
		ComPtr<ID3D12Resource2> m_BackBuffer;
		// Depth Buffer
		Scope<DirectX12DepthBuffer> m_DBuffer;
		
		bool m_Vsync;
		BOOL m_TearingSupport;
		HWND m_Window;

		static const int m_BufferCount = 2;
		
		Ref<DirectX12RenderTargetView> m_RTV[m_BufferCount];
		UINT m_CurrentBackBuffer;

		unsigned int m_Width;
		unsigned int m_Height;
		float m_ClearColor[4] = { 0.07f, 0.0f, 0.12f, 1 };
	};

};

