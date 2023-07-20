#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <queue>

namespace Render {
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
	class DirectXCommandQueue
	{
	public:
		DirectXCommandQueue(Microsoft::WRL::ComPtr<ID3D12Device8> device, D3D12_COMMAND_LIST_TYPE type);
		DirectXCommandQueue();

		virtual ~DirectXCommandQueue() = default;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> GetCommandList();

		UINT ExecuteCommandList(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> commandList);

		UINT Signal();
		bool IsFenceComplete(UINT fenceValue);
		void WaitForFenceValue(UINT fenceValue);
		void Flush();

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
	protected:
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator);
	private:
		struct CommandAllocatorEntry {
			UINT fenceValue;
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
		};

		using CommandAllocatorQueue = std::queue<CommandAllocatorEntry>;
		using CommandListQueue = std::queue<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6>>;

		D3D12_COMMAND_LIST_TYPE m_CommandListType;
		Microsoft::WRL::ComPtr<ID3D12Device8> m_Device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_Queue;
		Microsoft::WRL::ComPtr<ID3D12Fence1> m_Fence;
		HANDLE m_FenceEvent;
		UINT m_FenceValue;

		CommandAllocatorQueue m_CommandAllocatorQueue;
		CommandListQueue m_CommandListQueue;
	};
}


