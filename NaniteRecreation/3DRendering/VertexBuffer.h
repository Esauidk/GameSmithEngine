#pragma once
#include "Buffer.h"
namespace Render {
	template <typename T>
	class VertexBuffer : public Buffer<T> {
	public:
		VertexBuffer() = default;
		~VertexBuffer() = default;

		VertexBuffer(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> pCommandList, T* buffer, int count) : Buffer<T>(pDevice, pCommandList, buffer, count) {}

		//TODO: IMplement Binding
		void Bind() override {
			D3D12_VERTEX_BUFFER_VIEW view;
			view.BufferLocation = this->gpuBuffer->GetGPUVirtualAddress();
			view.SizeInBytes = this->bufferSize;
			view.StrideInBytes = sizeof(T);
			throw std::exception("Ran");
			return;
		}
	};
};

