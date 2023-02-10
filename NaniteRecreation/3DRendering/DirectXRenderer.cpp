#include "d3dx12.h"
#include "DirectXRenderer.h"
#include "DirectXMacros.h"
// Temporary Includes (Remove Vertex and Index)
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <chrono>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;
using namespace Render;


DirectXRenderer::DirectXRenderer(HWND hWnd): buffer(nullptr){
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
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 2;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	RENDER_THROW(pDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&pRTVHeapD)));

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

	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RENDER_THROW(pDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&pDSVHeapD)));

};

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
	queue.WaitForFenceValue(value);

	RENDER_THROW(pSwapChain->Present(0, 0));
	
	pSwapChain->GetBuffer(pSwapChain->GetCurrentBackBufferIndex(), IID_PPV_ARGS(&pBackBuffer));
	
}

void DirectXRenderer::CreateObject() {
	Vertex cubeVertex[] = {
		{DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)}
	};

	UINT indexCount[] = {
		1, 2, 3
	};

	// Test Code
	ComPtr<ID3D12GraphicsCommandList6> pCommandList = queue.GetCommandList();
	buffer = new VertexBuffer<Vertex>(pDevice, pCommandList, cubeVertex, 1);
	index = new IndexBuffer(pDevice, pCommandList, indexCount, 3);
	UINT value = queue.ExecuteCommandList(pCommandList);
	queue.WaitForFenceValue(value);
}

void DirectXRenderer::DrawObject() {
	buffer->Bind();
	index->Bind();
}
