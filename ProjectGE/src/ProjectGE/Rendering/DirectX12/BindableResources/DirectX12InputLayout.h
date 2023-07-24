#pragma once
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12PipelineDefiner.h"

namespace ProjectGE {
	class DirectX12InputLayout : public DirectX12PipelineDefiner
	{
	public:
		DirectX12InputLayout(const D3D12_INPUT_ELEMENT_DESC* elements, UINT count);
		void Setup(DirectX12PipelineState& pipeline) override;
	private:
		D3D12_INPUT_LAYOUT_DESC m_InputLayout;
	};
};


