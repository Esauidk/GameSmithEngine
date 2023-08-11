#include "gepch.h"
#include "DirectX12ShaderReference.h"

#include "ProjectGE/Rendering/DirectX12/DirectX12Context.h"

#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	DirectX12ShaderReference::DirectX12ShaderReference(UINT registerSlot, UINT size, D3D12_ROOT_DESCRIPTOR_FLAGS flags) : DirectX12ShaderInput(registerSlot, size)
	{
		m_Parameter.InitAsConstantBufferView(registerSlot,0, flags, D3D12_SHADER_VISIBILITY_VERTEX);
	}

	void DirectX12ShaderReference::SetData(void* rawData)
	{
		if (!m_DataSet) {
			m_DataSet = true;
		}

		m_RawData = rawData;
	}

	void DirectX12ShaderReference::Bind()
	{
		GE_CORE_ASSERT(m_DataSet, "Raw Data is not set on a shader constant");
		auto& cmdList = DirectX12Context::GetDirectCommandList();
		cmdList->SetGraphicsRootConstantBufferView(m_RegSlot, *((D3D12_GPU_VIRTUAL_ADDRESS*)m_RawData));

	}
};

