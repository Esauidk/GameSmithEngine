#include "InputLayout.h"

namespace Render {
	InputLayout::InputLayout(const D3D12_INPUT_ELEMENT_DESC layout) {
		inputLayout = layout;
	}

	//TODO: IMplement Binding
	void InputLayout::Bind() {
	
	}
};
