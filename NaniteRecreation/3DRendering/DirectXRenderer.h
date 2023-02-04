#pragma once
#include "Renderer.h"
#include "MainException.h"
#include "DirectXCommandQueue.h"
#include <vector>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

namespace Render {

	class DirectXRenderer : public Renderer
	{
	public:
		// Exceptions Possible for this class

		// Normal Exception
		class Exception : public MainException {
			using MainException::MainException;
		public:
			static std::string TranslateErrorCode(HRESULT hr) noexcept;
		};
		// Exceptions relating to HRESULTs
		class HrException : public Exception {
		public:
			HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsg = {}) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorDescription() const noexcept;
			std::string GetErrorInfo() const noexcept;
		private:
			HRESULT hr;
			std::string info;
		};
	public:
		DirectXRenderer(HWND hWnd);
		~DirectXRenderer();
		bool Initialize(HWND hWnd) override;
		void StartFrame() override;
		void EndFrame() override;
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
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> pRTVHeapD;

		// The command queue for the renderer
		DirectXCommandQueue queue;
		
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource2>> resources;
	};
};

