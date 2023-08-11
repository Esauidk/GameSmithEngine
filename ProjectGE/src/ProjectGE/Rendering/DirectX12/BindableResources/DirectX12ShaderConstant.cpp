#include "gepch.h"
#include "DirectX12ShaderConstant.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Context.h"

#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	DirectX12ShaderConstant::DirectX12ShaderConstant(UINT registerSlot, UINT count) :  DirectX12ShaderInput(registerSlot, count) {
		m_Parameter.InitAsConstants(count, registerSlot, 0, D3D12_SHADER_VISIBILITY_VERTEX);
	}

	void DirectX12ShaderConstant::SetData(void* rawData)
	{
		if (!m_DataSet) {
			m_DataSet = true;
		}

		m_RawData = rawData;
	}

	void DirectX12ShaderConstant::Bind()
	{
		GE_CORE_ASSERT(m_DataSet, "Raw Data is not set on a shader constant");
		auto& cmdList = DirectX12Context::GetDirectCommandList();
		cmdList->SetGraphicsRoot32BitConstants(m_RegSlot, m_Size, m_RawData, 0);
	}
};

