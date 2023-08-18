#pragma once

#include "DirectX12ShaderInput.h"

namespace ProjectGE {
	class DirectX12ShaderReference : public DirectX12ShaderInput
	{
	public:
		DirectX12ShaderReference(UINT registerSlot, D3D12_ROOT_DESCRIPTOR_FLAGS flags);
		//void SetData(void* rawData) override;
		//void Bind() override;
	};
};


