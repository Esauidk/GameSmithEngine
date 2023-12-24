#pragma once
#include "GameSmithEngine/Rendering/DirectX12/Util/third-party/d3dx12.h"

namespace GameSmith {
	class DirectX12BarrierWrapper {
	public:
		DirectX12BarrierWrapper(const CD3DX12_RESOURCE_BARRIER barrier) : m_LowLevelBarrier(barrier) {}
		CD3DX12_RESOURCE_BARRIER& GetBarrier() { return m_LowLevelBarrier; }
	private:
		CD3DX12_RESOURCE_BARRIER m_LowLevelBarrier;
	};
};
