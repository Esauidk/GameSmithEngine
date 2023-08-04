#include "gepch.h"
#include "DirectX12IndexBuffer.h"

namespace ProjectGE {
	DirectX12IndexBuffer::DirectX12IndexBuffer(WORD* buffer, int count) : DirectX12Buffer<WORD>::DirectX12Buffer(buffer, count, "Index Buffer"), m_Count(count) {}

	void DirectX12IndexBuffer::Bind() {
		DirectX12Context::InitializeQueueWait(DirectX12QueueType::Copy, DirectX12QueueType::Direct, m_UploadSignal);

		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			this->m_GpuBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER
		);
		auto& cmdList = DirectX12Context::GetDirectCommandList();
		cmdList->ResourceBarrier(1, &barrier);
		D3D12_INDEX_BUFFER_VIEW view;
		view.BufferLocation = this->m_GpuBuffer->GetGPUVirtualAddress();
		view.Format = DXGI_FORMAT_R16_UINT;
		view.SizeInBytes = this->m_BufferSize;
		
		cmdList->IASetIndexBuffer(&view);
	}
	UINT DirectX12IndexBuffer::GetCount()
	{
		return m_Count;
	}
};
