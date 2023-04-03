#include "InputLayout.h"

namespace Render {
	InputLayout::InputLayout(const D3D12_INPUT_ELEMENT_DESC* elements, UINT count) {
		inputLayout = { elements, count };
	}

	void InputLayout::Setup(PipelineState pipeline) {
		pipeline.Attach(inputLayout);
	}

	//TODO: IMplement Binding
	void InputLayout::Bind() {
	
	}
};
