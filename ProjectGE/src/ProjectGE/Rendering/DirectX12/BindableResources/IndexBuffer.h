#pragma once
#include "Buffer.h"

namespace ProjectGE {
	class IndexBuffer : public Buffer<WORD> {
	public:
		IndexBuffer() = default;

		IndexBuffer(Microsoft::WRL::ComPtr<ID3D12Device8> pDevice, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> pCommandList, WORD* buffer, int count);

		void Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) override;
	};
};
