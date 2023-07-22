#pragma once
#include "Buffer.h"

namespace ProjectGE {
	class IndexBuffer : public Buffer<WORD> {
	public:
		IndexBuffer() = default;

		IndexBuffer(ID3D12Device8* pDevice, ID3D12GraphicsCommandList6* pCommandList, WORD* buffer, int count);

		void Bind(ID3D12GraphicsCommandList6* cmdList) override;
	};
};
