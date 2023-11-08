#include "gepch.h"
#include "DirectX12Resource.h"

namespace GameSmith {
	DirectX12Resource::DirectX12Resource(ComPtr<ID3D12Resource> inResource, D3D12_RESOURCE_STATES initialState)
	{
		m_Resource = inResource;
		m_StateTracker.Init(initialState, m_Resource.Get());
	}

	DirectX12BarrierTracker& DirectX12Resource::GetResourceStateTracker()
	{
		return m_StateTracker;
	}
};