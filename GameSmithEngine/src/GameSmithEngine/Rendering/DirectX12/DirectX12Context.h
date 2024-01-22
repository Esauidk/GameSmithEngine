#pragma once

#include "gepch.h"
#include "GameSmithEngine/Rendering/RendererContext.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12DepthBuffer.h"
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12Views.h"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "GameSmithEngine/Core/Core.h"

using Microsoft::WRL::ComPtr;

namespace GameSmith {
	// Forward declaration
	class DirectX12RenderTexture;
	class DirectX12Resource;

	class DirectX12Context : public RendererContext
	{
	public:
		DirectX12Context(HWND window, unsigned int initialWidth, unsigned int initialHeight);
		~DirectX12Context() = default;
		void Init() override;
		bool Swap() override;
		void Resize(float width, float height, bool useForRender) override;
		void ResizeRenderSpace(float posX, float posY, float width, float height) override;
		void SetVsync(bool vsync) override;
		void SetClearColor(float r, float g, float b, float a) override;

	private:
		void InitializeBackBuffer();

		// Core Resources
		// This holds the two buffers we use for rendering (Presenting Buffer & Preparing Buffer)
		ComPtr<IDXGISwapChain4> m_SwapChain;
		Ref<DirectX12Resource> m_BackBuffer;
		// Depth Buffer
		Scope<DirectX12DepthBuffer> m_DBuffer;
		
		bool m_Vsync;
		BOOL m_TearingSupport;
		HWND m_Window;

		static const int m_BufferCount = 2;
		
		DirectX12RenderTargetView m_RTV[m_BufferCount];
		Ref<DirectX12RenderTexture> m_RenderTexture;
		DirectX12RenderTargetView m_RenderTextureView;
		UINT m_CurrentBackBuffer;

		unsigned int m_FrameWidth;
		unsigned int m_FrameHeight;

		unsigned int m_RenderPosX;
		unsigned int m_RenderPosY;
		unsigned int m_RenderWidth;
		unsigned int m_RenderHeight;
		float m_ClearColor[4] = { 0.07f, 0.0f, 0.12f, 1 };
	};

};

