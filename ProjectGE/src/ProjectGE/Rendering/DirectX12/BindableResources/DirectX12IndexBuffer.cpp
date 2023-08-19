#include "gepch.h"
#include "DirectX12IndexBuffer.h"

namespace ProjectGE {
	DirectX12IndexBuffer::DirectX12IndexBuffer(WORD* buffer, int count) : m_Buffer(std::make_unique<DirectX12Buffer<WORD>>(buffer, count, "Index Buffer")), m_Count(count) {}

	void DirectX12IndexBuffer::Bind() {
		m_Buffer->SetUploadGPUBlock();
		m_Buffer->TransitionState(D3D12_RESOURCE_STATE_INDEX_BUFFER);
		
		auto& cmdList = DirectX12Core::GetCore().GetDirectCommandContext()->GetCommandList();
		D3D12_INDEX_BUFFER_VIEW view;
		view.BufferLocation = m_Buffer->GetGPUReference();
		view.Format = DXGI_FORMAT_R16_UINT;
		view.SizeInBytes = m_Buffer->GetSize();
		
		cmdList->IASetIndexBuffer(&view);
	}
	UINT DirectX12IndexBuffer::GetCount()
	{
		return m_Count;
	}
};
