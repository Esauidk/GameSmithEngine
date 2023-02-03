#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <queue>

namespace Render {
	class DirectXCommandQueue
	{
	public:
		DirectXCommandQueue(Microsoft::WRL::ComPtr<ID3D12Device8> device, D3D12_COMMAND_LIST_TYPE type);
		DirectXCommandQueue();

		virtual ~DirectXCommandQueue();

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

		D3D12_COMMAND_LIST_TYPE commandListType;
		Microsoft::WRL::ComPtr<ID3D12Device8> pDevice;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> pQueue;
		Microsoft::WRL::ComPtr<ID3D12Fence1> pFence;
		HANDLE fenceEvent;
		UINT fenceValue;

		CommandAllocatorQueue commandAllocatorQueue;
		CommandListQueue commandListQueue;
	};
}


