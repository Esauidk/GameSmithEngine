#pragma once
#include "DirectX12Buffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/IndexBuffer.h"

namespace ProjectGE {
	class DirectX12IndexBuffer : public DirectX12Buffer<WORD>, public IndexBuffer {
	public:
		DirectX12IndexBuffer() = default;

		DirectX12IndexBuffer(WORD* buffer, int count);

		void Bind(ID3D12GraphicsCommandList6* cmdList) override;
	};
};
