#pragma once
#include "DirectX12Buffer.h"

namespace ProjectGE {
	class DirectX12IndexBuffer : public DirectX12Buffer<WORD> {
	public:
		DirectX12IndexBuffer() = default;

		DirectX12IndexBuffer(ID3D12Device8* pDevice, ID3D12GraphicsCommandList6* pCommandList, WORD* buffer, int count);

		void Bind(ID3D12GraphicsCommandList6* cmdList) override;
	};
};
