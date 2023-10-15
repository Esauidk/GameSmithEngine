#pragma once
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12Buffer.h"
#include "ProjectGE/Rendering/DirectX12/RenderComponents/DirectX12InputLayout.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/VertexBuffer.h"

namespace ProjectGE {
	class DirectX12VertexBuffer : public VertexBuffer {
	public:
		DirectX12VertexBuffer(BYTE* buffer, int vertexByteSize, int vertexCount) : m_Buffer(Scope<DirectX12Buffer<BYTE>>(new DirectX12Buffer<BYTE>(buffer, vertexByteSize * vertexCount, "Vertex Buffer"))), m_VertexByteSize(vertexByteSize) {}

		D3D12_VERTEX_BUFFER_VIEW GenerateView();

		virtual void UpdateData(BYTE* data, unsigned int size) override;

	private:
		Scope<DirectX12Buffer<BYTE>> m_Buffer;
		UINT m_VertexByteSize;
	};
};

