#include "gepch.h"
#include "DirectX12CommandQueue.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"
#include "GameSmithEngine/Core/Log.h"

using namespace Microsoft::WRL;
using namespace std::chrono;

namespace GameSmith {
	DirectX12CommandQueue::DirectX12CommandQueue(DirectX12QueueType type) :
		m_FenceValue(0) {

		auto& core = DirectX12Core::GetCore();
		m_Device = core.GetDevice();
		switch (type) {
		case DirectX12QueueType::Direct:
			m_CommandListType = D3D12_COMMAND_LIST_TYPE_DIRECT;
			break;
		case DirectX12QueueType::Copy:
			m_CommandListType = D3D12_COMMAND_LIST_TYPE_COPY;
			break;
		default:
			GE_CORE_ASSERT(false, "Unsupported command list type");
		}

		D3D12_COMMAND_QUEUE_DESC commandDesc = {};
		commandDesc.Type = m_CommandListType;
		commandDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		commandDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		commandDesc.NodeMask = 0;

		bool res = FAILED(m_Device->CreateCommandQueue(&commandDesc, IID_PPV_ARGS(&m_Queue)));
		GE_CORE_ASSERT(!res, "Failed to create a command queue");

		res = FAILED(m_Device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
		GE_CORE_ASSERT(!res, "Failed to create a fence");

		m_FenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

		GE_CORE_ASSERT(m_FenceEvent, "Failed to create fence event");

		Signal();
	}

	ID3D12CommandAllocator* DirectX12CommandQueue::CreateCommandAllocator() {
		ID3D12CommandAllocator* commandAllocator;
		bool res = FAILED(m_Device->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&commandAllocator)));
		GE_CORE_ASSERT(!res, "Failed to create command allocator of type {0}", m_CommandListType);

		return commandAllocator;
	}

	ID3D12GraphicsCommandList6* DirectX12CommandQueue::CreateCommandList(ID3D12CommandAllocator* allocator) {
		ID3D12GraphicsCommandList6* commandList;
		bool res = FAILED(m_Device->CreateCommandList(0, m_CommandListType, allocator, nullptr, IID_PPV_ARGS(&commandList)));
		GE_CORE_ASSERT(!res, "Failed to create a command list of type {0}", m_CommandListType);

		return commandList;
	}

	ID3D12CommandQueue* DirectX12CommandQueue::GetCommandQueue() const {
		return m_Queue.Get();
	}

	DirectX12CommandListWrapper* DirectX12CommandQueue::GetCommandList() {
		ComPtr<ID3D12GraphicsCommandList6> commandList;
		ComPtr<ID3D12CommandAllocator> commandAllocator;

		bool res;
		if (!m_CommandAllocatorQueue.empty() && IsFenceComplete(m_CommandAllocatorQueue.front().fenceValue)) {
			commandAllocator = m_CommandAllocatorQueue.front().commandAllocator;
			m_CommandAllocatorQueue.pop();

			res = FAILED(commandAllocator->Reset());
			GE_CORE_ASSERT(!res, "Failed to reset a command list");
		}
		else {
			commandAllocator = CreateCommandAllocator();
		}

		if (!m_CommandListQueue.empty()) {
			commandList = m_CommandListQueue.front();
			m_CommandListQueue.pop();

			res = FAILED(commandList->Reset(commandAllocator.Get(), nullptr));
			GE_CORE_ASSERT(!res, "Failed to reset a command list");
		}
		else {
			commandList = CreateCommandList(commandAllocator.Get());
		}

		res = FAILED(commandList->SetPrivateDataInterface(__uuidof(ID3D12CommandAllocator), commandAllocator.Get()));
		GE_CORE_ASSERT(!res, "Failed to assign command list to command allocator");

		return new DirectX12CommandListWrapper(commandList);
	}

	UINT DirectX12CommandQueue::ExecuteCommandList(DirectX12CommandListWrapper& commandList) {
		commandList.CloseList();

		ID3D12CommandAllocator* commandAllocator;
		UINT dataSize = sizeof(commandAllocator);
		bool res = FAILED(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));
		GE_CORE_ASSERT(!res, "Failed to find command allocator assign to this command list")


		ID3D12CommandList* const commandLists[]{
			&commandList
		};

		m_Queue->ExecuteCommandLists(1, commandLists);
		UINT fenceValue = Signal();

		m_CommandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator});
		m_CommandListQueue.push(&commandList);

		commandAllocator->Release();

		return fenceValue;
	}

	UINT DirectX12CommandQueue::Signal() {
		m_FenceValue++;
		bool res = FAILED(m_Queue->Signal(m_Fence.Get(), m_FenceValue));
		GE_CORE_ASSERT(!res, "Failed to signal fence");
		return m_FenceValue;
	}

	bool DirectX12CommandQueue::IsFenceComplete(UINT fenceValue) {
		return m_Fence->GetCompletedValue() >= fenceValue;
	}

	void DirectX12CommandQueue::CPUWaitForFenceValue(UINT fenceValue) {
		if (m_Fence->GetCompletedValue() < fenceValue) {
			bool res = FAILED(m_Fence->SetEventOnCompletion(fenceValue, m_FenceEvent));
			GE_CORE_ASSERT(!res, "Failed to assign fence completion event");
			::WaitForSingleObject(m_FenceEvent, (DWORD)((milliseconds::max)().count()));
		}
	}

	void DirectX12CommandQueue::GPUWaitForFenceValue(UINT fenceValue)
	{
		m_Queue->Wait(m_Fence.Get(), fenceValue);
	}

	void DirectX12CommandQueue::GPUWaitForFenceValue(DirectX12CommandQueue& otherQueue, UINT fenceValue)
	{
		m_Queue->Wait(otherQueue.m_Fence.Get(), fenceValue);
	}

	void DirectX12CommandQueue::Flush() {
		// Note: There used to be a signal call there, moved to constructor to make sure a fence value always has a signal
		CPUWaitForFenceValue(m_FenceValue);
	}

};
