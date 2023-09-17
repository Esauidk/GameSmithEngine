#pragma once
#include "ProjectGE/Rendering/DirectX12/Resources/Util/DirectX12BarrierTracker.h"
#include <wrl.h>
#include <d3d12.h>

using Microsoft::WRL::ComPtr;

namespace ProjectGE {
	class DirectX12Resource {
	public:
		DirectX12Resource(ComPtr<ID3D12Resource> inResource, D3D12_RESOURCE_STATES initialState);
		DirectX12BarrierTracker& GetResourceStateTracker();
		ID3D12Resource* GetResource() { return m_Resource.Get(); }
	private:
		ComPtr<ID3D12Resource> m_Resource;
		DirectX12BarrierTracker m_StateTracker;
	};
};
