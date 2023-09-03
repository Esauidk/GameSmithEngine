#include "gepch.h"
#include "DirectX12VertexBuffer.h"

namespace ProjectGE {
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
};