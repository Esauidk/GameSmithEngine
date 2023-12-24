#include "gepch.h"
#include "DirectX12BarrierTracker.h"

#include <d3d12.h>
#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"
#include "GameSmithEngine/Rendering/DirectX12/Resources/Util/DirectX12BarrierWrapper.h"

#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {

	void DirectX12BarrierTracker::Init(D3D12_RESOURCE_STATES initialState, ID3D12Resource* resource)
	{
		m_CurrentState = initialState;
		m_LastState = initialState;
		m_Resource = resource;
		m_IsInit = true;
	}

	void DirectX12BarrierTracker::TransitionBarrier(D3D12_RESOURCE_STATES nextState, DirectX12CommandContextBase& context)
	{
		GE_CORE_ASSERT(m_IsInit, "Transition Barrier Has Not Been Initialized Yet");

		if (m_CurrentState != nextState) {
			auto& core = DirectX12Core::GetCore();
			auto& cmdList = core.GetDirectCommandContext().GetCommandList();
			DirectX12BarrierWrapper barrier(CD3DX12_RESOURCE_BARRIER::Transition(
				m_Resource,
				m_CurrentState, nextState
			));

			context.InsertBarrier(barrier);
			m_LastState = m_CurrentState;
			m_CurrentState = nextState;
		}
	}

	void DirectX12BarrierTracker::UndoTransition(DirectX12CommandContextBase& context)
	{
		TransitionBarrier(m_LastState, context);
	}
};

