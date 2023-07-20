#include "DirectXCommandQueue.h"
#include "DirectXMacros.h"
#include <chrono>

using namespace Render;
using namespace Microsoft::WRL;
using namespace std::chrono;

DirectXCommandQueue::DirectXCommandQueue(ComPtr<ID3D12Device8> device, D3D12_COMMAND_LIST_TYPE type): 
	m_FenceValue(0), 
	m_CommandListType(type), 
	m_Device(device) {

	D3D12_COMMAND_QUEUE_DESC commandDesc = {};
	commandDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandDesc.NodeMask = 0;
	RENDER_THROW(m_Device->CreateCommandQueue(&commandDesc, IID_PPV_ARGS(&m_Queue)));

	RENDER_THROW(m_Device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));

	m_FenceEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_FenceEvent) {
		throw std::exception("Failed to create fence event");
	}
}

DirectXCommandQueue::DirectXCommandQueue() {}

ComPtr<ID3D12CommandAllocator> DirectXCommandQueue::CreateCommandAllocator() {
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	RENDER_THROW(m_Device->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&commandAllocator)));

	return commandAllocator;
}

ComPtr<ID3D12GraphicsCommandList6> DirectXCommandQueue::CreateCommandList(ComPtr<ID3D12CommandAllocator> allocator) {
	ComPtr<ID3D12GraphicsCommandList6> commandList;
	RENDER_THROW(m_Device->CreateCommandList(0, m_CommandListType, allocator.Get(), nullptr, IID_PPV_ARGS(&commandList)));

	return commandList;
}

ComPtr<ID3D12CommandQueue> DirectXCommandQueue::GetCommandQueue() const {
	return m_Queue;
}

ComPtr<ID3D12GraphicsCommandList6> DirectXCommandQueue::GetCommandList() {
	ComPtr<ID3D12GraphicsCommandList6> commandList;
	ComPtr<ID3D12CommandAllocator> commandAllocator;

	if (!m_CommandAllocatorQueue.empty() && IsFenceComplete(m_CommandAllocatorQueue.front().fenceValue)) {
		commandAllocator = m_CommandAllocatorQueue.front().commandAllocator;
		m_CommandAllocatorQueue.pop();

		RENDER_THROW(commandAllocator->Reset());
	}
	else {
		commandAllocator = CreateCommandAllocator();
	}

	if (!m_CommandListQueue.empty()) {
		commandList = m_CommandListQueue.front();
		m_CommandListQueue.pop();
		
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

	m_Queue->ExecuteCommandLists(1, commandLists);
	UINT fenceValue = Signal();

	m_CommandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator });
	m_CommandListQueue.push(commandList);

	commandAllocator->Release();

	return fenceValue;
}

UINT DirectXCommandQueue::Signal() {
	m_FenceValue++;
	RENDER_THROW(m_Queue->Signal(m_Fence.Get(), m_FenceValue));
	return m_FenceValue;
}

bool DirectXCommandQueue::IsFenceComplete(UINT fenceValue) {
	return m_Fence->GetCompletedValue() >= fenceValue;
}

void DirectXCommandQueue::WaitForFenceValue(UINT fenceValue) {
	if (m_Fence->GetCompletedValue() < fenceValue) {
		RENDER_THROW(m_Fence->SetEventOnCompletion(fenceValue, m_FenceEvent));
		::WaitForSingleObject(m_FenceEvent, (milliseconds::max)().count());
	}
}

void DirectXCommandQueue::Flush() {
	Signal();
	WaitForFenceValue(m_FenceValue);
}