#pragma once
#include "PipelineDefiner.h"

namespace Render {
	class InputLayout : public PipelineDefiner
	{
	public:
		InputLayout(const D3D12_INPUT_ELEMENT_DESC* elements, UINT count);
		void Setup(PipelineState& pipeline) override;
		void Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) override;
	private:
		D3D12_INPUT_LAYOUT_DESC inputLayout;
	};
};


