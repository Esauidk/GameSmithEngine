#include "gepch.h"
#include "DirectX12CommandContext.h"

#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {

	void DirectX12CommandContextBase::StartCommandList()
	{
		if (m_CurrentList != nullptr) {
			GE_CORE_ASSERT(false, "Command List is already open! Close this list if it is meant to be finished");
		}

		m_CurrentList = Scope<DirectX12CommandListWrapper>(m_Queue->GetCommandList());
		if (m_StateManager != nullptr) {
			m_StateManager->NewCommandList();
		}
		
	}

	UINT DirectX12CommandContextBase::FinalizeCommandList()
	{
		if (m_CurrentList == nullptr) {
			GE_CORE_ASSERT(false, "Command List is already closed! Open a new list");
		}

		//TODO: Create a execution system for the command lists (seperate thread), would be great to execute batches at the same time
		UINT val = m_Queue->ExecuteCommandList(*(m_CurrentList.get()));
		m_CurrentList.release();

		m_CurrentList = nullptr;

		StartCommandList();
		return val;
	}

	DirectX12CommandContextBase::DirectX12CommandContextBase(DirectX12QueueType type) : m_QueueType(type)
	{
		m_Queue = Scope<DirectX12CommandQueue>(new DirectX12CommandQueue(type));
		m_CurrentList = Scope<DirectX12CommandListWrapper>(m_Queue->GetCommandList());

		if (type == DirectX12QueueType::Direct) {
			m_StateManager = Scope<DirectX12StateManager>(new DirectX12StateManager(type));
		}
		
	}
};

