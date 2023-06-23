#pragma once
#include "ProjectGE/Rendering/DirectX12/BindableResources/PipelineDefiner.h"

namespace ProjectGE {
	class InputLayout : public PipelineDefiner
	{
	public:
		InputLayout(const D3D12_INPUT_ELEMENT_DESC* elements, UINT count);
		void Setup(PipelineState& pipeline) override;
		void Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) override;
	private:
		D3D12_INPUT_LAYOUT_DESC m_InputLayout;
	};
};


