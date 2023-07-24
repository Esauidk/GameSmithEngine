#include "gepch.h"
#include "DirectX12InputLayout.h"

namespace ProjectGE {
	DirectX12InputLayout::DirectX12InputLayout(const D3D12_INPUT_ELEMENT_DESC* elements, UINT count) : m_InputLayout({ elements, count }) {}

	void DirectX12InputLayout::Setup(DirectX12PipelineState& pipeline) {
		pipeline.Attach(m_InputLayout);
	}
};
