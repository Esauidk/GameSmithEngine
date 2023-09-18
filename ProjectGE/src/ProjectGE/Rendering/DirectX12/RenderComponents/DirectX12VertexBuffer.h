#pragma once
#include "ProjectGE/Rendering/DirectX12/Resources/DirectX12Buffer.h"
#include "ProjectGE/Rendering/DirectX12/RenderComponents/DirectX12InputLayout.h"

#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/VertexBuffer.h"

namespace ProjectGE {
	class DirectX12VertexBuffer : public VertexBuffer {
	public:
		DirectX12VertexBuffer(BYTE* buffer, int vertexByteSize, int vertexCount) : m_Buffer(std::make_unique<DirectX12Buffer<BYTE>>(buffer, vertexByteSize * vertexCount, "Vertex Buffer")), m_VertexByteSize(vertexByteSize) {}

		D3D12_VERTEX_BUFFER_VIEW GenerateView();

		inline void AttachLayout(const BufferLayoutBuilder& layout) override {
			m_Layout.reset();
			m_Layout = std::make_unique<DirectX12InputLayout>(layout);
		}

		inline PipelineDefiner* GetLayout() override {
			return m_Layout.get();
		}

	private:
		Scope<DirectX12Buffer<BYTE>> m_Buffer;
		Scope<DirectX12InputLayout> m_Layout;
		UINT m_VertexByteSize;
	};
};

