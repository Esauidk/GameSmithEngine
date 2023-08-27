#pragma once

#include <wrl.h>
#include <d3d12.h>

#include "ProjectGE/Core/Core.h"
#include "ProjectGE/Rendering/DirectX12/CommandList/DirectX12CommandContext.h"

using Microsoft::WRL::ComPtr;

namespace ProjectGE {
	class DirectX12Core {
	public:
		static DirectX12Core& CreateCore();
		static DirectX12Core& GetCore();
		inline ID3D12Device8* GetDevice() { return m_Device.Get(); }
		inline DirectX12CommandContextDirect* GetDirectCommandContext() { return m_DirectContext.get(); }
		inline DirectX12CommandContextCopy* GetCopyCommandContext() { return m_CopyContext.get(); }
		// Tell one queue to wait for another queue to complete a certain amount of work
		void InitializeQueueWait(DirectX12QueueType executor, DirectX12QueueType waiter, UINT fenceVal);
		// Tell the CPU to wait for all work to complete on a specific queue
		void InitializeCPUQueueWait(DirectX12QueueType target);
		// Tell the CPU to wait for some work to be complete on a specific queue
		void InitializeCPUQueueWait(UINT fenceVal, DirectX12QueueType target);
	private:
		DirectX12Core();
		void Init();
		DirectX12CommandQueue& FindQueue(DirectX12QueueType type);
	private:
		ComPtr<ID3D12Debug> m_Debug;
		ComPtr<ID3D12InfoQueue> m_InfoQueue;
		ComPtr<ID3D12Device8> m_Device;
		Scope<DirectX12CommandContextDirect> m_DirectContext;
		Scope<DirectX12CommandContextCopy> m_CopyContext;
		static Scope<DirectX12Core> m_Core;
	};
};
