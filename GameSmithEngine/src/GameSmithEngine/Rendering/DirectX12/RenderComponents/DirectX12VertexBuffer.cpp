#include "gepch.h"
#include "DirectX12VertexBuffer.h"

#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	D3D12_VERTEX_BUFFER_VIEW DirectX12VertexBuffer::GenerateView()
	{
		m_Buffer->SetUploadGPUBlock();
		m_Buffer->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

		D3D12_VERTEX_BUFFER_VIEW view;
		view.BufferLocation = m_Buffer->GetGPUReference();
		view.SizeInBytes = m_Buffer->GetSize();
		view.StrideInBytes = m_VertexByteSize;

		return view;
	}

	void DirectX12VertexBuffer::UpdateData(BYTE* data, unsigned int size)
	{
		GE_CORE_ERROR("VertexBuffer: updating data not currently supported");
	}
};