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
	DirectXRenderer(HWND hWnd);
	~DirectXRenderer();
	bool Initialize(HWND hWnd) override;
	void StartFrame() override;
	void EndFrame() override;
private:
	// Debug Resources
	Microsoft::WRL::ComPtr<ID3D12Debug> pDebug;
	Microsoft::WRL::ComPtr<ID3D12InfoQueue> pInfoQueue;

	// Core Resources
	Microsoft::WRL::ComPtr<ID3D12Device> pDevice;
	// This holds the two buffers we use for rendering (Presenting Buffer & Preparing Buffer)
	Microsoft::WRL::ComPtr<IDXGISwapChain4> pSwapChain;

	// Graphics Handlers
	Microsoft::WRL::ComPtr<ID3D12Resource2> pBackBuffer;
	D3D12_CPU_DESCRIPTOR_HANDLE targetHandler;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> pRTVHeapD;

	// Command System
	// 
	// GPU Side
	// This is the GPU side storage of the queues to execute, all commands planned to be execute wil be on this queue on the GPU
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> pCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12Fence1> pFence;
	// CPU Side
	// This can be understood as the interface/API for command lists, it interaces with the memory backing command allocator
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList2> pCommandList;
	// This handles the allocation of space for commands in the command list, this is the real storage of commands
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> pCommandAlloc;
};

