#include "gepch.h"
#include "DirectX12ShaderReferenceTable.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Context.h"

#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	DirectX12ShaderReferenceTable::DirectX12ShaderReferenceTable(UINT registerSlot, UINT count) : DirectX12ShaderInput(registerSlot, count){
		
	}

	void DirectX12ShaderReferenceTable::Bind()
	{
		//TODO: IMPLEMENT
		GE_CORE_ASSERT(false, "Not yet implemented");
	}
};

