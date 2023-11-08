#pragma once
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12Buffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/IndexBuffer.h"

namespace GameSmith {
	class DirectX12IndexBuffer : public IndexBuffer {
	public:
		DirectX12IndexBuffer() = default;

		DirectX12IndexBuffer(unsigned int* buffer, unsigned int count);

		D3D12_INDEX_BUFFER_VIEW GenerateView();

		UINT GetCount() override;
	private:
		Scope<DirectX12Buffer<unsigned int>> m_Buffer;
		UINT m_Count;
	};
};
