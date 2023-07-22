#pragma once
#include "Buffer.h"

namespace ProjectGE {
	template <typename T>
	class VertexBuffer : public Buffer<T> {
	public:
		VertexBuffer() = default;
		~VertexBuffer() = default;

		VertexBuffer(ID3D12Device8* pDevice, ID3D12GraphicsCommandList6* pCommandList, T* buffer, int count) : Buffer<T>(pDevice, pCommandList, buffer, count, "Vertex Buffer") {}


		void Bind(ID3D12GraphicsCommandList6* cmdList) override {
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
				this->m_GpuBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
			);
			cmdList->ResourceBarrier(1, &barrier);
			D3D12_VERTEX_BUFFER_VIEW view;
			view.BufferLocation = this->m_GpuBuffer->GetGPUVirtualAddress();
			view.SizeInBytes = this->m_BufferSize;
			view.StrideInBytes = sizeof(T);
			
			cmdList->IASetVertexBuffers(0, 1, &view);
		}
	};
};

