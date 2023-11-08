#pragma once
#include "gepch.h"
#include "GameSmithEngine/Rendering/DirectX12/CommandList/DirectX12CommandQueue.h"
#include "GameSmithEngine/Rendering/DirectX12/State/DirectX12StateManager.h"
#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12HeapDatabase.h"

#include "GameSmithEngine/Core/Core.h"

namespace GameSmith {
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


