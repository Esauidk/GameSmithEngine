#include "d3dx12.h"
#include "DirectXRenderer.h"
#include "DirectXMacros.h"
#include <DirectXMath.h>
#include <chrono>
#include <sstream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;
using namespace Render;


DirectXRenderer::DirectXRenderer(HWND hWnd){
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

	queue = DirectXCommandQueue(pDevice, D3D12_COMMAND_LIST_TYPE_DIRECT);

	/*// Gets our command queue, is used to send commands to GPU (draw calls, copy call, compute calls) [Lives on GPU]
	D3D12_COMMAND_QUEUE_DESC commandDesc = {};
	commandDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandDesc.NodeMask = 0;

	RENDER_THROW(pDevice->CreateCommandQueue(&commandDesc, IID_PPV_ARGS(&pCommandQueue)));*/
	
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
		queue.GetCommandQueue().Get(),
		hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&dxgiSwapChain1));

	// Disables auto-fullscreen (Alt-Enter)
	RENDER_THROW(dxgiFactory5->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER));
	// Upgrades swap chan to SwapChain4
	RENDER_THROW(dxgiSwapChain1.As(&pSwapChain));

	// Describe a heap of descriptors (Need one for each back buffer since they will have their own target render views)
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 2;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	RENDER_THROW(pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&pRTVHeapD)));

	UINT rtvSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// Gets descriptor for render target view
	CD3DX12_CPU_DESCRIPTOR_HANDLE targetHandler = CD3DX12_CPU_DESCRIPTOR_HANDLE(pRTVHeapD->GetCPUDescriptorHandleForHeapStart());
	
	// Assign target views to buffers and their place in the heap
	for (int i = 0; i < 2; i++) {
		RENDER_THROW(pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer)));
		pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, targetHandler);
		targetHandler.Offset(rtvSize);
	}

	RENDER_THROW(pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)));

};
DirectXRenderer::~DirectXRenderer() {};

bool DirectXRenderer::Initialize(HWND hWnd) {
	
	return true;
}

void DirectXRenderer::StartFrame() {
	// Reseting Render Target
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		pBackBuffer.Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	ComPtr<ID3D12GraphicsCommandList6> list = queue.GetCommandList();
	// Add command to transition the render target
	list->ResourceBarrier(1, &barrier);
	UINT index = pSwapChain->GetCurrentBackBufferIndex();
	CD3DX12_CPU_DESCRIPTOR_HANDLE targetHandler(pRTVHeapD->GetCPUDescriptorHandleForHeapStart(), pSwapChain->GetCurrentBackBufferIndex(), pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	const float color[] = { 0.07f, 0.0f, 0.12f, 1 };
	list->ClearRenderTargetView(targetHandler, color, 0, nullptr);
	UINT value = queue.ExecuteCommandList(list);
	queue.WaitForFenceValue(value);

}

void DirectXRenderer::EndFrame() {
	// Present Render Target
	CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		pBackBuffer.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT
	);
	ComPtr<ID3D12GraphicsCommandList6> list = queue.GetCommandList();
	// Add command to transition the render target
	list->ResourceBarrier(1, &barrier);
	
	UINT value = queue.ExecuteCommandList(list);

	RENDER_THROW(pSwapChain->Present(0, 0));
	
	pSwapChain->GetBuffer(pSwapChain->GetCurrentBackBufferIndex(), IID_PPV_ARGS(&pBackBuffer));
	queue.WaitForFenceValue(value);
}

void DirectXRenderer::DrawObject() {
	struct Vertex {
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 color;
	};

	Vertex cubeVertex[] = {
		{DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)}
	};

	const UINT vertexBufferSize = sizeof(cubeVertex);

	CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_HEAP_PROPERTIES uploadHeapProperties(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC vertexBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
	ComPtr<ID3D12Resource2> pVertexBuffer;
	ComPtr<ID3D12Resource2> pUploadVertexBuffer;

	RENDER_THROW(pDevice->CreateCommittedResource(
		&defaultHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&pVertexBuffer)
	));

	RENDER_THROW(pDevice->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&vertexBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&pUploadVertexBuffer)
	));

	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = reinterpret_cast<BYTE*>(cubeVertex);
	vertexData.RowPitch = vertexBufferSize;
	vertexData.SlicePitch = vertexData.RowPitch;

	UpdateSubresources(queue.GetCommandList().Get(), pVertexBuffer.Get(), pUploadVertexBuffer.Get(), 0, 0, 1, &vertexData);

	resources.push_back(pVertexBuffer);
	resources.push_back(pUploadVertexBuffer);
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
