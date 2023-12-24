#include "gepch.h"
#include "DirectX12CommandContext.h"

#include "GameSmithEngine/Core/Log.h"

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
		m_Queue.ExecuteCommandLists(m_CompletedLists);
	}

	UINT DirectX12CommandContextBase::FinalizeCommandList()
	{
		if (!m_CurrentList.isOpen()) {
			GE_CORE_ASSERT(false, "Command List is already closed! Open a new list");
		}
		
		FinalizeResourceBarriers();

		m_CurrentList.CloseList();
		//TODO: Create a execution system for the command lists (seperate thread), would be great to execute batches at the same time
		UINT val = m_Queue.ExecuteCommandList(m_CurrentList);
		
		StartCommandList();
		return val;
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

