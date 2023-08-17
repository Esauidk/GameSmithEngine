#pragma once
#include "DirectX12ShaderInput.h"

namespace ProjectGE {
	class DirectX12ShaderReferenceTable : public DirectX12ShaderInput
	{
		DirectX12ShaderReferenceTable(ShaderArguementType tableType, UINT registerSlot, UINT size);
		void Bind() override;
	};
};


