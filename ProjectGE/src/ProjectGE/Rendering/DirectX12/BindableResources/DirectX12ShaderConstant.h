#pragma once

#include "DirectX12ShaderInput.h"

namespace ProjectGE {
	class DirectX12ShaderConstant : public DirectX12ShaderInput
	{
	public:
		DirectX12ShaderConstant(UINT registerSlot, UINT count);
		//void SetData(void* rawData) override;
		//void Bind() override;
	};
};


