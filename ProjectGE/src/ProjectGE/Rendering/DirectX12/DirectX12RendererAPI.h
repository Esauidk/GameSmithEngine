#pragma once

#include "DirectX12RendererHeaders.h"
#include "ProjectGE/Rendering/RendererAPI.h"

namespace ProjectGE {
	class DirectX12RendererAPI : public RendererAPI
	{
	public:
		DirectX12RendererAPI();
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(Ref<GeometryPack> geopack) override;

		void FinishRecording() override;


		virtual Ref<VertexBuffer> CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount) override;
		virtual void SetVertexBuffer(Ref<VertexBuffer> vbuffer) override;
		virtual Ref<IndexBuffer> CreateIndexBuffer(unsigned int* data, unsigned int indexCount) override;
		virtual void SetIndexBuffer(Ref<IndexBuffer> ibuffer) override;
		virtual Ref<Shader> LoadShader() override;
		virtual void SubmitRecording() override;
	private:
		DirectX12Core& m_Core;
	};
};


