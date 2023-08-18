#include "gepch.h"
#include "DirectX12ShaderReferenceTable.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Context.h"

#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	DirectX12ShaderReferenceTable::DirectX12ShaderReferenceTable( UINT registerSlot, UINT count) : DirectX12ShaderInput(registerSlot, count){
		
		//m_Parameter.InitAsDescriptorTable(3, &range, )
	}	

	/*void DirectX12ShaderReferenceTable::Bind()
	{
		//TODO: IMPLEMENT
		GE_CORE_ASSERT(false, "Not yet implemented");
		auto& cmdList = DirectX12Context::GetDirectCommandList();
	}*/
};

