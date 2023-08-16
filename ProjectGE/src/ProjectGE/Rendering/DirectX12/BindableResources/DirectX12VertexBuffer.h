#pragma once
#include "ProjectGE/Rendering/DirectX12/DirectX12Buffer.h"
#include "DirectX12InputLayout.h"

#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/VertexBuffer.h"

namespace ProjectGE {
	template <typename T>
	class DirectX12VertexBuffer : public VertexBuffer {
	public:
		DirectX12VertexBuffer(T* buffer, int count) : m_Buffer(std::make_unique<DirectX12Buffer<T>>(buffer, count, "Vertex Buffer")) {}


		void Bind() override {
			m_Buffer->SetUploadGPUBlock();
			m_Buffer->TransitionState(D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

			D3D12_VERTEX_BUFFER_VIEW view;
			view.BufferLocation = m_Buffer->GetGPUReference();
			view.SizeInBytes = m_Buffer->GetSize();
			view.StrideInBytes = sizeof(T);
			
			auto& cmdList = DirectX12Context::GetDirectCommandList();
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
		Scope<DirectX12Buffer<T>> m_Buffer;
		Scope<DirectX12InputLayout> m_Layout;
	};
};

