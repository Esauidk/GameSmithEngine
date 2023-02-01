#include "d3dx12.h"
#include "DirectXRenderer.h"
#include "DirectXMacros.h"
#include <sstream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

DirectXRenderer::DirectXRenderer(HWND hWnd) {
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

	// Gets our command queue, is used to send commands to GPU (draw calls, copy call, compute calls) [Lives on GPU]
	D3D12_COMMAND_QUEUE_DESC commandDesc = {};
	commandDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandDesc.NodeMask = 0;

	RENDER_THROW(pDevice->CreateCommandQueue(&commandDesc, IID_PPV_ARGS(&pCommandQueue)));
	
	/**********************************************/
	// SWAP-CHAIN CREATION
	/*********************************************/

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

	// Disables auto-fullscreen (Alt-Enter)
	RENDER_THROW(dxgiFactory5->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));
	// Upgrades swap chan to SwapChain4
	RENDER_THROW(dxgiSwapChain1.As(&pSwapChain));

	// Creates a heap of descriptors (going to be used to create the descriptor of the render target view)
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	RENDER_THROW(pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&pRTVHeapD)));

	// Need size if you have more than one back buffer
	//rtvSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// Gets descriptor for render target view
	targetHandler = D3D12_CPU_DESCRIPTOR_HANDLE(pRTVHeapD->GetCPUDescriptorHandleForHeapStart());
	
	RENDER_THROW(pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));

	pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, targetHandler);

	// Creates the command list, where commands live on the CPU side
	RENDER_THROW(pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&pCommandAlloc)));

	// Creates the command list for CPU
	RENDER_THROW(pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, pCommandAlloc.Get(), nullptr, IID_PPV_ARGS(&pCommandList)));

	RENDER_THROW(pCommandList->Close());

	// Create Fence for the GPU to ensure synchronization
	RENDER_THROW(pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pFence)));

	HANDLE fenceEvent;

	fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!fenceEvent) {
		throw std::exception("Failed to create fence event");
	}

};
DirectXRenderer::~DirectXRenderer() {};

bool DirectXRenderer::Initialize(HWND hWnd) {
	
	return true;
}

void DirectXRenderer::StartFrame() {
	// Resetting Command List and Allocator
	pCommandAlloc->Reset();
	pCommandList->Reset(pCommandAlloc.Get(), nullptr);

	// Reseting Render Target
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		pBackBuffer.Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
	);

	// Add command to transition the render target
	pCommandList->ResourceBarrier(1, &barrier);

	const float color[] = { 0.07f, 0.0f, 0.12f, 1 };
	pCommandList->ClearRenderTargetView(targetHandler, color, 0, nullptr);
}

void DirectXRenderer::EndFrame() {
	// Present Render Target
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		pBackBuffer.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT
	);

	// Add command to transition the render target
	pCommandList->ResourceBarrier(1, &barrier);

	RENDER_THROW(pCommandList->Close());

	ID3D12CommandList* const commandLists[] = { pCommandList.Get() };
	pCommandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

	RENDER_THROW(pSwapChain->Present(0, 0));
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
