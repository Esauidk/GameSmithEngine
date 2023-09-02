#pragma once
#include "ProjectGE/Rendering/DirectX12/Util/DirectX12Buffer.h"
#include "ProjectGE/Rendering/DirectX12/BindableResources/DirectX12InputLayout.h"

#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/VertexBuffer.h"

namespace ProjectGE {
	class DirectX12VertexBuffer : public VertexBuffer {
	public:
		DirectX12VertexBuffer(BYTE* buffer, int vertexByteSize, int vertexCount) : m_Buffer(std::make_unique<DirectX12Buffer<BYTE>>(buffer, vertexByteSize * vertexCount, "Vertex Buffer")), m_VertexByteSize(vertexByteSize) {}


		void Bind() override {
			m_Buffer->SetUploadGPUBlock();
			m_Buffer->GetStateTracker().TransitionBarrier(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

			D3D12_VERTEX_BUFFER_VIEW view;
			view.BufferLocation = m_Buffer->GetGPUReference();
			view.SizeInBytes = m_Buffer->GetSize();
			view.StrideInBytes = m_VertexByteSize;
			
			auto& cmdList = DirectX12Core::GetCore().GetDirectCommandContext().GetCommandList();
			cmdList->IASetVertexBuffers(0, 1, &view);
		}

		void AttachLayout(const BufferLayoutBuilder& layout) override {
			m_Layout.reset();
			m_Layout = std::make_unique<DirectX12InputLayout>(layout);
		}

		PipelineDefiner* GetLayout() override {
			return m_Layout.get();
		}

	private:
		Scope<DirectX12Buffer<BYTE>> m_Buffer;
		Scope<DirectX12InputLayout> m_Layout;
		UINT m_VertexByteSize;
	};
};

