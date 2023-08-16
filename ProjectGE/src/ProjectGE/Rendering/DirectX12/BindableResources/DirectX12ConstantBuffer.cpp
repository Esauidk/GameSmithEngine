#include "gepch.h"
#include "DirectX12ConstantBuffer.h"

namespace ProjectGE {
	void ProjectGE::DirectX12ConstantBuffer::UpdateData(BYTE* data)
	{
		if (m_Buffer->GetState() != D3D12_RESOURCE_STATE_COPY_DEST) {
			m_Buffer->TransitionState(D3D12_RESOURCE_STATE_COPY_DEST);
		}

		m_Buffer->UpdateData(data);
	}

	void* ProjectGE::DirectX12ConstantBuffer::GetGPUReference()
	{
		if (m_Buffer->GetState() == D3D12_RESOURCE_STATE_COPY_DEST) {
			m_Buffer->SetUploadGPUBlock();
			m_Buffer->TransitionState(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		}

		return &m_GPUAdd;
	}
};

