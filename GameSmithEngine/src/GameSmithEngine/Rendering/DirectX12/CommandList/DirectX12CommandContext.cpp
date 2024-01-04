#include "gepch.h"
#include "DirectX12CommandContext.h"

#include "GameSmithEngine/Core/Log.h"

#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"

namespace GameSmith {

	void DirectX12CommandContextBase::StartCommandList()
	{
		if (m_CurrentList.isOpen()) {
			GE_CORE_ASSERT(false, "Command List is already open! Close this list if it is meant to be finished");
		}

		m_CurrentList = m_Queue.GetCommandList();
		if (m_QueueType == DirectX12QueueType::Direct) {
			m_StateManager.NewCommandList();
		}
		
	}

	void DirectX12CommandContextBase::SubmitCommandLists()
	{
		m_LastSignal = m_Queue.ExecuteCommandLists(m_CompletedLists);

		auto& core = DirectX12Core::GetCore();
		for (unsigned int type = 0; type < DirectX12QueueType::NUM_QUEUES; type++) {
			if (m_WaitRequests[type]) {
				core.InitializeQueueWait(m_QueueType, (DirectX12QueueType)type,  m_LastSignal);
				m_WaitRequests[type] = false;
			}
		}

		m_CompletedLists.clear();
	}

	void DirectX12CommandContextBase::FinalizeCommandList()
	{
		if (!m_CurrentList.isOpen()) {
			GE_CORE_ASSERT(false, "Command List is already closed! Open a new list");
		}
		
		FinalizeResourceBarriers();

		m_CurrentList.CloseList();
		m_CompletedLists.push_back(m_CurrentList);
		
		StartCommandList();
	}

	void DirectX12CommandContextBase::FinalizeResourceBarriers()
	{
		if (!m_Barriers.empty()) {
			std::vector<CD3DX12_RESOURCE_BARRIER> barriers;
			for (auto wrapper : m_Barriers) {
				barriers.push_back(wrapper.GetBarrier());
			}

			m_CurrentList->ResourceBarrier((UINT)barriers.size(), barriers.data());
		}

		m_Barriers.clear();
	}

	DirectX12CommandContextBase::DirectX12CommandContextBase(DirectX12QueueType type) : m_QueueType(type), m_Queue(type), m_CurrentList(m_Queue.GetCommandList()), m_StateManager(DirectX12QueueType::Direct)
	{
	}
};

