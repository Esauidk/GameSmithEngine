#pragma once

#include "DirectX12RendererHeaders.h"
#include "ProjectGE/Rendering/RendererAPI.h"

namespace ProjectGE {
	class DirectX12RendererAPI : public RendererAPI
	{
	public:
		DirectX12RendererAPI();
		virtual API GetAPI() override { return API::DirectX12; }

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(UINT indecies, UINT instances) override;

		virtual Ref<VertexBuffer> CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount) override;
		virtual void SetVertexBuffer(Ref<VertexBuffer> vbuffer) override;
		virtual Ref<IndexBuffer> CreateIndexBuffer(unsigned int* data, unsigned int indexCount) override;
		virtual void SetIndexBuffer(Ref<IndexBuffer> ibuffer) override;
		virtual Ref<Shader> LoadShader(std::string path) override;

		virtual Ref<ConstantBuffer> CreateConstantBuffer(UINT size) override;
		virtual void SetConstantBuffer(Ref<ConstantBuffer> cbuffer, Stages stage, ShaderConstantType constantType) override;

		virtual Ref<Texture2D> CreateTexture2D(char* data, UINT size) override;
		virtual void SetTexture2D(Ref<Texture2D> tex, Stages stage) override;

		virtual Ref<Sampler> CreateSampler(FilterType img, PaddingMethod padMode) override;
		virtual void SetSampler(Ref<Sampler> sampler, Stages stage) override;

		virtual void SetTopology(TopologyType& type) override;

		virtual void UpdatePipeline(PipelineStateInitializer& init) override;

		virtual void SubmitRecording() override;
	private:
		DirectX12Core& m_Core;
	};
};


