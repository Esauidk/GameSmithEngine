#pragma once
#include "Renderer.h"
#include "MainException.h"
#include <vector>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

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
	DirectXRenderer();
	~DirectXRenderer();
	bool Initialize(HWND hWnd) override;
	void Render() override;
private:
	// Debug Resources
	Microsoft::WRL::ComPtr<ID3D12Debug> pDebug;
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> pInfoQueue;

	// Core Resources
	Microsoft::WRL::ComPtr<ID3D12Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> pCommandQueue;
	Microsoft::WRL::ComPtr< IDXGISwapChain4> pSwapChain;
};

