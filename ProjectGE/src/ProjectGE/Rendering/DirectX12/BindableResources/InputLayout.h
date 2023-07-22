#pragma once
#include "ProjectGE/Rendering/DirectX12/BindableResources/PipelineDefiner.h"

namespace ProjectGE {
	class InputLayout : public PipelineDefiner
	{
	public:
		InputLayout(const D3D12_INPUT_ELEMENT_DESC* elements, UINT count);
		void Setup(PipelineState& pipeline) override;
	private:
		D3D12_INPUT_LAYOUT_DESC m_InputLayout;
	};
};


