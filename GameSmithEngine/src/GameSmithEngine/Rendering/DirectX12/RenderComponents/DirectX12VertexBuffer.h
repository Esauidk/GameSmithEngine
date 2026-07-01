#pragma once
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12DedicatedBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/VertexBuffer.h"

namespace GameSmith {
	class DirectX12VertexBuffer : public VertexBuffer {
	public:
		DirectX12VertexBuffer(BYTE* buffer, int vertexByteSize, int vertexCount) : m_Buffer(Scope<DirectX12DedicatedBuffer<BYTE>>(new DirectX12DedicatedBuffer<BYTE>(buffer, vertexByteSize * vertexCount, "Vertex Buffer"))), m_VertexByteSize(vertexByteSize) {}

		D3D12_VERTEX_BUFFER_VIEW GenerateView();

		virtual void UpdateData(BYTE* data, unsigned int size) override;

	private:
		Scope<DirectX12DedicatedBuffer<BYTE>> m_Buffer;
		UINT m_VertexByteSize;
	};
};

