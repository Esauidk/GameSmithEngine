#pragma once
#include "BindableResource.h"
#include <d3d12.h>

namespace Render {
	class InputLayout : public BindableResource
	{
	public:
		InputLayout(const D3D12_INPUT_ELEMENT_DESC layout);
		void Bind() override;
	private:
		D3D12_INPUT_ELEMENT_DESC inputLayout;
	};
};


