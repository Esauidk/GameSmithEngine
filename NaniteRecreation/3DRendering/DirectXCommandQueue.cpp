#include "DirectXCommandQueue.h"
#include "DirectXMacros.h"
#include "DirectXRenderer.h"
#include <chrono>

using namespace Render;
using namespace Microsoft::WRL;
using namespace std::chrono;

DirectXCommandQueue::DirectXCommandQueue(ComPtr<ID3D12Device8> device, D3D12_COMMAND_LIST_TYPE type): 
	fenceValue(0), 
	commandListType(type), 
	pDevice(device) {

	D3D12_COMMAND_QUEUE_DESC commandDesc = {};
	commandDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandDesc.NodeMask = 0;
	RENDER_THROW(pDevice->CreateCommandQueue(&commandDesc, IID_PPV_ARGS(&pQueue)));

	RENDER_THROW(pDevice->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pFence)));

	fenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!fenceEvent) {
		throw std::exception("Failed to create fence event");
	}
}

DirectXCommandQueue::DirectXCommandQueue() {}
DirectXCommandQueue::~DirectXCommandQueue() {}

ComPtr<ID3D12CommandAllocator> DirectXCommandQueue::CreateCommandAllocator() {
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	RENDER_THROW(pDevice->CreateCommandAllocator(commandListType, IID_PPV_ARGS(&commandAllocator)));

	return commandAllocator;
}

ComPtr<ID3D12GraphicsCommandList6> DirectXCommandQueue::CreateCommandList(ComPtr<ID3D12CommandAllocator> allocator) {
	ComPtr<ID3D12GraphicsCommandList6> commandList;
	RENDER_THROW(pDevice->CreateCommandList(0, commandListType, allocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

	return commandList;
}

ComPtr<ID3D12CommandQueue> DirectXCommandQueue::GetCommandQueue() const {
	return pQueue;
}

ComPtr<ID3D12GraphicsCommandList6> DirectXCommandQueue::GetCommandList() {
	ComPtr<ID3D12GraphicsCommandList6> commandList;
	ComPtr<ID3D12CommandAllocator> commandAllocator;

	if (!commandAllocatorQueue.empty() && IsFenceComplete(commandAllocatorQueue.front().fenceValue)) {
		commandAllocator = commandAllocatorQueue.front().commandAllocator;
		commandAllocatorQueue.pop();

		RENDER_THROW(commandAllocator->Reset());
	}
	else {
		commandAllocator = CreateCommandAllocator();
	}

	if (!commandListQueue.empty()) {
		commandList = commandListQueue.front();
		commandListQueue.pop();
		
		RENDER_THROW(commandList->Reset(commandAllocator.Get(), nullptr));
	}
	else {
		commandList = CreateCommandList(commandAllocator);
	}

	RENDER_THROW(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAllocator.Get()));

	return commandList;
}

UINT DirectXCommandQueue::ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList6> commandList) {
	commandList->Close();
	
	ID3D12CommandAllocator* commandAllocator;
	UINT dataSize = sizeof(commandAllocator);
	RENDER_THROW(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));

	ID3D12CommandList* const commandLists[]{
		commandList.Get()
	};

	pQueue->ExecuteCommandLists(1, commandLists);
	UINT fenceValue = Signal();

	commandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator });
	commandListQueue.push(commandList);

	commandAllocator->Release();

	return fenceValue;
}

UINT DirectXCommandQueue::Signal() {
	fenceValue++;
	RENDER_THROW(pQueue->Signal(pFence.Get(), fenceValue));
	return fenceValue;
}

bool DirectXCommandQueue::IsFenceComplete(UINT fenceValue) {
	return pFence->GetCompletedValue() >= fenceValue;
}

void DirectXCommandQueue::WaitForFenceValue(UINT fenceValue) {
	if (pFence->GetCompletedValue() < fenceValue) {
		RENDER_THROW(pFence->SetEventOnCompletion(fenceValue, fenceEvent));
		::WaitForSingleObject(fenceEvent, (milliseconds::max)().count());
	}
}

void DirectXCommandQueue::Flush() {
	Signal();
	WaitForFenceValue(fenceValue);
}