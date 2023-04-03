#pragma once
#include "PipelineDefiner.h"
#include <d3d12.h>

namespace Render {
	class InputLayout : public PipelineDefiner
	{
	public:
		InputLayout(const D3D12_INPUT_ELEMENT_DESC* elements, UINT count);
		void Setup(PipelineState pipeline) override;
		void Bind() override;
	private:
		D3D12_INPUT_LAYOUT_DESC inputLayout;
	};
};


