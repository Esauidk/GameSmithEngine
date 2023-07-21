#include "gepch.h"
#include "DirectXCommandQueue.h"
#include "ProjectGE/Log.h"

using namespace Microsoft::WRL;
using namespace std::chrono;

namespace ProjectGE {
	DirectXCommandQueue::DirectXCommandQueue(ID3D12Device8* device, D3D12_COMMAND_LIST_TYPE type) :
		m_CommandListType(type),
		m_Device(device),
		m_FenceValue(0) {

		D3D12_COMMAND_QUEUE_DESC commandDesc = {};
		commandDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		commandDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		commandDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		commandDesc.NodeMask = 0;

		bool res = FAILED(m_Device->CreateCommandQueue(&commandDesc, IID_PPV_ARGS(&m_Queue)));
		GE_CORE_ASSERT(!res, "Failed to create a command queue");

		res = FAILED(m_Device->CreateFence(m_FenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
		GE_CORE_ASSERT(!res, "Failed to create a fence");

		m_FenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);

		GE_CORE_ASSERT(m_FenceEvent, "Failed to create fence event");
	}

	ID3D12CommandAllocator* DirectXCommandQueue::CreateCommandAllocator() {
		ID3D12CommandAllocator* commandAllocator;
		bool res = FAILED(m_Device->CreateCommandAllocator(m_CommandListType, IID_PPV_ARGS(&commandAllocator)));
		GE_CORE_ASSERT(!res, "Failed to create command allocator of type {0}", m_CommandListType);

		return commandAllocator;
	}

	ID3D12GraphicsCommandList6* DirectXCommandQueue::CreateCommandList(ID3D12CommandAllocator* allocator) {
		ID3D12GraphicsCommandList6* commandList;
		bool res = FAILED(m_Device->CreateCommandList(0, m_CommandListType, allocator, nullptr, IID_PPV_ARGS(&commandList)));
		GE_CORE_ASSERT(!res, "Failed to create a command list of type {0}", m_CommandListType);

		return commandList;
	}

	ID3D12CommandQueue* DirectXCommandQueue::GetCommandQueue() const {
		return m_Queue.Get();
	}

	ComPtr<ID3D12GraphicsCommandList6> DirectXCommandQueue::GetCommandList() {
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

		return commandList;
	}

	UINT DirectXCommandQueue::ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList6> commandList) {
		commandList->Close();

		ID3D12CommandAllocator* commandAllocator;
		UINT dataSize = sizeof(commandAllocator);
		bool res = FAILED(commandList->GetPrivateData(__uuidof(ID3D12CommandAllocator), &dataSize, &commandAllocator));
		GE_CORE_ASSERT(!res, "Failed to find command allocator assign to this command list")


		ID3D12CommandList* const commandLists[]{
			commandList.Get()
		};

		m_Queue->ExecuteCommandLists(1, commandLists);
		UINT fenceValue = Signal();

		m_CommandAllocatorQueue.emplace(CommandAllocatorEntry{ fenceValue, commandAllocator});
		m_CommandListQueue.push(commandList);

		commandAllocator->Release();

		return fenceValue;
	}

	UINT DirectXCommandQueue::Signal() {
		m_FenceValue++;
		bool res = FAILED(m_Queue->Signal(m_Fence.Get(), m_FenceValue));
		GE_CORE_ASSERT(!res, "Failed to signal fence");
		return m_FenceValue;
	}

	bool DirectXCommandQueue::IsFenceComplete(UINT fenceValue) {
		return m_Fence->GetCompletedValue() >= fenceValue;
	}

	void DirectXCommandQueue::WaitForFenceValue(UINT fenceValue) {
		if (m_Fence->GetCompletedValue() < fenceValue) {
			bool res = FAILED(m_Fence->SetEventOnCompletion(fenceValue, m_FenceEvent));
			GE_CORE_ASSERT(!res, "Failed to assign fence completion event");
			::WaitForSingleObject(m_FenceEvent, (DWORD)((milliseconds::max)().count()));
		}
	}

	void DirectXCommandQueue::Flush() {
		Signal();
		WaitForFenceValue(m_FenceValue);
	}
};
