#pragma once
#include "Buffer.h"

namespace Render {
	class IndexBuffer : public Buffer<UINT> {
	public:
		IndexBuffer() = default;
		~IndexBuffer() = default;

		IndexBuffer(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> pCommandList, UINT* buffer, int count);

		void Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) override;
	};
};
