#pragma once

#include "DirectX12RendererHeaders.h"
#include "GameSmithEngine/Rendering/RendererAPI.h"

namespace GameSmith {
	class DirectX12RendererAPI : public RendererAPI
	{
	public:
		DirectX12RendererAPI();
		inline virtual API GetAPI() override { return API::DirectX12; }

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(UINT indecies, UINT instances) override;

		virtual Ref<VertexBuffer> CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount) override;
		virtual void SetVertexBuffer(Ref<VertexBuffer> vbuffer) override;
		virtual Ref<IndexBuffer> CreateIndexBuffer(unsigned int* data, unsigned int indexCount) override;
		virtual void SetIndexBuffer(Ref<IndexBuffer> ibuffer) override;
		virtual Ref<Shader> LoadShader(std::string path) override;
		virtual Ref<Shader> LoadShader(char* byteCode, unsigned int length) override;

		virtual Ref<ConstantBuffer> CreateConstantBuffer(UINT size, std::string name) override;
		virtual Ref<ConstantBuffer> CreateConstantBuffer(UINT size) override;
		virtual void SetConstantBuffer(Ref<ConstantBuffer> cbuffer, Stages stage, ShaderConstantType constantType) override;

		virtual Ref<Texture2D> CreateTexture2D(char* data, UINT size) override;
		virtual void SetTexture2D(Ref<Texture2D> tex, UINT slot, Stages stage) override;

		virtual Ref<Sampler> CreateSampler(FilterType img, PaddingMethod padMode) override;
		virtual void SetSampler(Ref<Sampler> sampler, Stages stage) override;

		virtual void SetTopology(TopologyType& type, bool tesselation) override;

		virtual Ref<GraphicsPipelineStateObject> CreateGraphicsPipelineState(PipelineStateInitializer& init) override;
		virtual void SetGraphicsPipelineState(Ref<GraphicsPipelineStateObject> pso) override;

		virtual void SubmitRecording() override;

		inline virtual void ClearCachedAssets() override { m_SavedPipelineObjects.clear(); }
	private:
		DirectX12Core& m_Core;
		std::vector<Ref<DirectX12GraphicsPipelineState>> m_SavedPipelineObjects;
	};
};


