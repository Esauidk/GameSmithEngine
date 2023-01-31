#include "DirectXRenderer.h"
#include "DirectXMacros.h"
#include <sstream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

DirectXRenderer::DirectXRenderer() {
	// Create the Debug Layer (Allows debuging on the device)
#if defined(_DEBUG)
	RENDER_THROW(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebug)));

	pDebug->EnableDebugLayer();
#endif

	// Create Device
	RENDER_THROW(D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&pDevice)));
	
#if defined(_DEBUG)
	// Create Info Queue, shows debug logs depending on severity and sets breaks
	RENDER_THROW(pDevice.As(&pInfoQueue));

	pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);


	pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);


	pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, TRUE);
#endif

	// Gets our command queue, is used to send commands to GPU (draw calls, copy call, compute calls)
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = 0;

	RENDER_THROW(pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&pCommandQueue)));
	
};
DirectXRenderer::~DirectXRenderer() {};

bool DirectXRenderer::Initialize(HWND hWnd) {
	/**********************************************/
	// SWAP-CHAIN CREATION
	/*********************************************/

	ComPtr<IDXGISwapChain4> dxgiSwapChain4;
	ComPtr<IDXGIFactory5> dxgiFactory5;

	UINT createFactoryFlags = 0;

#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
	RENDER_THROW(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory5)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

	// Make them native size for now (potentially allow resolution changing in the future)
	swapChainDesc.Width = 0;
	swapChainDesc.Height = 0;

	// 8 Bytes per channel, unormalized
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Full-screen mode?
	swapChainDesc.Stereo = FALSE;
	// One Sampling, Normal Quality
	swapChainDesc.SampleDesc = { 1,0 };
	// Buffer used for rending to output (i.e. monitor/application)
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// The amount fo buffers in the swap chain
	swapChainDesc.BufferCount = 2;
	// Stretch if buffer doesn't fit resolution
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	// Discard contents of back buffer after presentation call (Fresh new buffer after each presentation call)
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	// Nothing really done with alphas
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	BOOL tearingSupport = FALSE;
	if (FAILED(dxgiFactory5->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING,
		&tearingSupport,
		sizeof(tearingSupport)))) {
		tearingSupport = FALSE;
	}

	swapChainDesc.Flags = (tearingSupport == TRUE) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	ComPtr<IDXGISwapChain1> dxgiSwapChain1;

	// Create OS tied Swap-Chain
	RENDER_THROW(dxgiFactory5->CreateSwapChainForHwnd(
		pCommandQueue.Get(),
		hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&dxgiSwapChain1));

	RENDER_THROW(dxgiFactory5->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));
	RENDER_THROW(dxgiSwapChain1.As(&pSwapChain));

	return true;
}

void DirectXRenderer::Render() {
}

DirectXRenderer::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept : Exception(line, file), hr(hr) {
	for (const auto& m : infoMsgs) {
		info += m;
		info.push_back('\n');
	}

	if (!info.empty()) {
		info.pop_back();
	}
}

const char* DirectXRenderer::HrException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty()) {
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::string DirectXRenderer::HrException::GetErrorInfo() const noexcept {
	return info;
}
const char* DirectXRenderer::HrException::GetType() const noexcept {
	return "Graphics HResult Exception";
}

HRESULT DirectXRenderer::HrException::GetErrorCode() const noexcept { return hr; }

std::string DirectXRenderer::HrException::GetErrorDescription() const noexcept {
	return TranslateErrorCode(hr);
}

std::string DirectXRenderer::Exception::TranslateErrorCode(HRESULT hr) noexcept {
	char* pMsgBuf = nullptr;
	return std::system_category().message(hr);
}
