#pragma once
#include "gepch.h"
#include "GameSmithEngine/Rendering/DirectX12/CommandList/DirectX12CommandQueue.h"
#include "GameSmithEngine/Rendering/DirectX12/State/DirectX12StateManager.h"
#include "GameSmithEngine/Rendering/DirectX12/HeapStructures/DirectX12HeapDatabase.h"

#include "GameSmithEngine/Rendering/DirectX12/Resources/Util/DirectX12BarrierWrapper.h"

#include "GameSmithEngine/Core/Core.h"

namespace GameSmith {
	// This class manages the lifetime and usage of command lists for DX12 in this engine
	class DirectX12CommandContextBase
	{
	public:
		void SubmitCommandLists();
		void FinalizeCommandList();
		void FinalizeResourceBarriers();
		inline void InsertBarrier(DirectX12BarrierWrapper& barrier) { m_Barriers.push_back(barrier); }
		inline void RequestWait(DirectX12QueueType type) { m_WaitRequests[type] = true; }
		inline DirectX12CommandListWrapper& GetCommandList() { return m_CurrentList; };
		inline DirectX12QueueType GetQueueType() const { return m_QueueType; }
		inline DirectX12CommandQueue& GetQueue() { return m_Queue; }
		inline DirectX12StateManager& GetStateManager() { return m_StateManager; }
		inline UINT GetLastSubmissionID() { return m_LastSignal; }
	protected:
		DirectX12CommandContextBase(DirectX12QueueType type);
	private:
		void StartCommandList();
		DirectX12QueueType m_QueueType;
		DirectX12CommandQueue m_Queue;
		DirectX12CommandListWrapper m_CurrentList;
		bool m_WaitRequests[DirectX12QueueType::NUM_QUEUES] = { false };
		std::vector<DirectX12BarrierWrapper> m_Barriers;
		DirectX12StateManager m_StateManager;
		std::vector<DirectX12CommandListWrapper> m_CompletedLists;
		UINT m_LastSignal = 0;
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


