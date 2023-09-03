#include "gepch.h"
#include "DirectX12IndexBuffer.h"

namespace ProjectGE {
	DirectX12IndexBuffer::DirectX12IndexBuffer(unsigned int* buffer, unsigned int count) : m_Buffer(std::make_unique<DirectX12Buffer<unsigned int>>(buffer, count, "Index Buffer")), m_Count(count) {}

	D3D12_INDEX_BUFFER_VIEW DirectX12IndexBuffer::GenerateView()
	{
		m_Buffer->SetUploadGPUBlock();
		m_Buffer->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_INDEX_BUFFER);

		D3D12_INDEX_BUFFER_VIEW view;
		view.BufferLocation = m_Buffer->GetGPUReference();
		view.Format = DXGI_FORMAT_R16_UINT;
		view.SizeInBytes = m_Buffer->GetSize();

		return view;
	}

	UINT DirectX12IndexBuffer::GetCount()
	{
		return m_Count;
	}
};
