#pragma once
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12Buffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/IndexBuffer.h"

namespace ProjectGE {
	class DirectX12IndexBuffer :  public IndexBuffer {
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
