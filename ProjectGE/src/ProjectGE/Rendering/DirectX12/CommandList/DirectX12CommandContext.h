#pragma once
#include "gepch.h"
#include "ProjectGE/Rendering/DirectX12/CommandList/DirectX12CommandQueue.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12StateManager.h"
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12HeapDatabase.h"

#include "ProjectGE/Core/Core.h"

namespace ProjectGE {
	// This class manages the lifetime and usage of command lists for DX12 in this engine
	class DirectX12CommandContextBase
	{
	public:
		UINT FinalizeCommandList();
		inline DirectX12CommandListWrapper& GetCommandList() { return *(m_CurrentList.get()); };
		inline DirectX12QueueType GetQueueType() const { return m_QueueType; }
		inline DirectX12CommandQueue& GetQueue() { return *(m_Queue.get()); }
		inline DirectX12StateManager& GetStateManager() { return *m_StateManager; }
	protected:
		DirectX12CommandContextBase(DirectX12QueueType type);
	private:
		void StartCommandList();
		Scope<DirectX12CommandListWrapper> m_CurrentList;
		Scope<DirectX12CommandQueue> m_Queue;
		std::queue<DirectX12CommandListWrapper> m_CompletedLists;
		DirectX12QueueType m_QueueType;
		Scope<DirectX12StateManager> m_StateManager;
	};

	class DirectX12CommandContextDirect : public DirectX12CommandContextBase {
	public:
		inline DirectX12CommandContextDirect() : DirectX12CommandContextBase(DirectX12QueueType::Direct) {}
	};

	class DirectX12CommandContextCopy : public DirectX12CommandContextBase {
	public:
		inline DirectX12CommandContextCopy() : DirectX12CommandContextBase(DirectX12QueueType::Copy) {}
	};
};


