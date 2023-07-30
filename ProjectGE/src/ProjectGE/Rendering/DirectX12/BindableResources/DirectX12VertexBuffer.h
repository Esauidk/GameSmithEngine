#pragma once
#include "DirectX12Buffer.h"
#include "DirectX12InputLayout.h"

#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/VertexBuffer.h"

namespace ProjectGE {
	template <typename T>
	class DirectX12VertexBuffer : public DirectX12Buffer<T>, public VertexBuffer {
	public:
		DirectX12VertexBuffer(T* buffer, int count) : DirectX12Buffer<T>(buffer, count, "Vertex Buffer") {}


		void Bind(ID3D12GraphicsCommandList6* cmdList) override {
			DirectX12Renderer::SyncJob(this->m_UploadSignal, D3D12_COMMAND_LIST_TYPE_COPY);

			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
				this->m_GpuBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
			);
			cmdList->ResourceBarrier(1, &barrier);
			D3D12_VERTEX_BUFFER_VIEW view;
			view.BufferLocation = this->m_GpuBuffer->GetGPUVirtualAddress();
			view.SizeInBytes = this->m_BufferSize;
			view.StrideInBytes = sizeof(T);
			
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
		std::unique_ptr<DirectX12InputLayout> m_Layout;
	};
};

