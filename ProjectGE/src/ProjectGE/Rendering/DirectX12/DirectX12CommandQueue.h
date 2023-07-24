#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <queue>

using Microsoft::WRL::ComPtr;

namespace ProjectGE {
	/* 
	DirectXCommandQueue: An abstraction of the command system from DirectX12, synchronize compatible
	Description: Handles overhead involving command queue's and their respective commandlist and allocators

	Command System 
		GPU Side
		Command Queue: This is the GPU side storage of the queues to execute, all commands planned to be execute wil be on this queue on the GPU

		CPU Side
		Command List: This can be understood as the interface/API for command lists, it interaces with the memory backing command allocator
		Command Allocator: This handles the allocation of space for commands in the command list, this is the real storage of commands

	*/ 
	class DirectX12CommandQueue
	{
	public:
		DirectX12CommandQueue(ID3D12Device8* device, D3D12_COMMAND_LIST_TYPE type);
		DirectX12CommandQueue() = delete;

		~DirectX12CommandQueue() = default;

		ComPtr<ID3D12GraphicsCommandList6> GetCommandList();

		UINT ExecuteCommandList(ComPtr<ID3D12GraphicsCommandList6> commandList);

		UINT Signal();
		bool IsFenceComplete(UINT fenceValue);
		void WaitForFenceValue(UINT fenceValue);
		void Flush();

		// Return a ptr to the the underlying command queue, keep ownership of it
		ID3D12CommandQueue* GetCommandQueue() const;
	protected:
		// Create an allocator, but do not own the resulting ptr, the client owns it
		ID3D12CommandAllocator* CreateCommandAllocator();
		// Create a command list, but do not own the resulting ptr, the client owns it
		ID3D12GraphicsCommandList6* CreateCommandList(ID3D12CommandAllocator* allocator);
	private:
		struct CommandAllocatorEntry {
			UINT fenceValue;
			ComPtr<ID3D12CommandAllocator> commandAllocator;
		};

		using CommandAllocatorQueue = std::queue<CommandAllocatorEntry>;
		using CommandListQueue = std::queue<ComPtr<ID3D12GraphicsCommandList6>>;

		D3D12_COMMAND_LIST_TYPE m_CommandListType;
		ID3D12Device8* m_Device;
		ComPtr<ID3D12CommandQueue> m_Queue;
		ComPtr<ID3D12Fence1> m_Fence;
		HANDLE m_FenceEvent;
		UINT m_FenceValue;

		CommandAllocatorQueue m_CommandAllocatorQueue;
		CommandListQueue m_CommandListQueue;
	};
}


