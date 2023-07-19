#include "gepch.h"
#include "IndexBuffer.h"

namespace ProjectGE {
	IndexBuffer::IndexBuffer(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> pCommandList, WORD* buffer, int count) : Buffer<WORD>::Buffer(pDevice, pCommandList, buffer, count) {}

	void IndexBuffer::Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) {
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
			this->m_GpuBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER
		);
		cmdList->ResourceBarrier(1, &barrier);
		D3D12_INDEX_BUFFER_VIEW view;
		view.BufferLocation = this->m_GpuBuffer->GetGPUVirtualAddress();
		view.Format = DXGI_FORMAT_R16_UINT;
		view.SizeInBytes = this->m_BufferSize;
		
		cmdList->IASetIndexBuffer(&view);
	}
};
