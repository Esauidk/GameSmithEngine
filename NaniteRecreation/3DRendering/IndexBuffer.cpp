#include "IndexBuffer.h"

namespace Render {
	IndexBuffer::IndexBuffer(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> pCommandList, UINT* buffer, int count) : Buffer<UINT>::Buffer(pDevice, pCommandList, buffer, count) {}

	//TODO: IMplement Binding
	void IndexBuffer::Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) {
		D3D12_INDEX_BUFFER_VIEW view;
		view.BufferLocation = this->gpuBuffer->GetGPUVirtualAddress();
		view.Format = DXGI_FORMAT_R16_UINT;
		view.SizeInBytes = this->bufferSize;
		
		cmdList->IASetIndexBuffer(&view);
	}
};
