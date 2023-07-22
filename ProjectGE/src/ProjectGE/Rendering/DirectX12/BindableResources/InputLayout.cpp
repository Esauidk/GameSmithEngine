#include "gepch.h"
#include "InputLayout.h"

namespace ProjectGE {
	InputLayout::InputLayout(const D3D12_INPUT_ELEMENT_DESC* elements, UINT count) : m_InputLayout({ elements, count }) {}

	void InputLayout::Setup(PipelineState& pipeline) {
		pipeline.Attach(m_InputLayout);
	}
};
