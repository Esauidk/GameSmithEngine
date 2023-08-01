#include "gepch.h"
#include "DirectX12ShaderConstant.h"

namespace ProjectGE {
	DirectX12ShaderConstant::DirectX12ShaderConstant(UINT registerSlot, void* data, UINT size) : m_RawData(data), m_RegSlot(registerSlot), m_Size(size) {
		m_Parameter.InitAsConstants(size, registerSlot, D3D12_SHADER_VISIBILITY_VERTEX);
	}

	DirectX12ShaderConstant::~DirectX12ShaderConstant()
	{
		delete m_RawData;
	}

	void DirectX12ShaderConstant::Bind(ID3D12GraphicsCommandList6* cmdList)
	{
		cmdList->SetGraphicsRoot32BitConstants(m_RegSlot, m_Size, m_RawData, 0);
	}
};
