#pragma once

#include "DirectX12RendererHeaders.h"
#include "GameSmithEngine/Rendering/RendererAPI.h"

namespace GameSmith {
	class DirectX12RendererAPI : public RendererAPI
	{
	public:
		DirectX12RendererAPI();
		~DirectX12RendererAPI() override;
		inline API GetAPI() override { return API::DirectX12; }

		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(UINT indecies, UINT instances) override;

		Ref<VertexBuffer> CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount) override;
		void SetVertexBuffer(Ref<VertexBuffer> vbuffer) override;
		Ref<IndexBuffer> CreateIndexBuffer(unsigned int* data, unsigned int indexCount) override;
		void SetIndexBuffer(Ref<IndexBuffer> ibuffer) override;
		Ref<Shader> LoadShader(std::string path) override;
		Ref<Shader> LoadShader(const char* byteCode, unsigned int length) override;
		Ref<const char> CompileShader(const Stages stage, const char* rawCode, const unsigned int length, const char* entryPt, ShaderIncludeCache* includeCache, unsigned int* outSize) override;

		Ref<ConstantBuffer> CreateConstantBuffer(UINT size, std::string name) override;
		Ref<ConstantBuffer> CreateConstantBuffer(UINT size) override;
		void SetConstantBuffer(Ref<ConstantBuffer> cbuffer, Stages stage, ShaderConstantType constantType) override;

		Ref<Texture2D> CreateTexture2D(char* data, UINT size) override;
		Ref<RenderTexture> CreateRenderTexture(unsigned int width, unsigned int height) override;
		Ref<RenderTexture> CreateRenderTexture(unsigned int width, unsigned int height, float* clearColor) override;;

		void SetRenderTexture(Ref<RenderTexture> rt, UINT index) override;
		void SetTexture2D(Ref<Texture2D> tex, UINT slot, Stages stage) override;

		Ref<Sampler> CreateSampler(FilterType img, PaddingMethod padMode) override;
		void SetSampler(Ref<Sampler> sampler, Stages stage) override;

		void SetTopology(TopologyType& type, bool tesselation) override;

		Ref<GraphicsPipelineStateObject> CreateGraphicsPipelineState(PipelineStateInitializer& init) override;
		void SetGraphicsPipelineState(Ref<GraphicsPipelineStateObject> pso) override;

		void SubmitRecording() override;
		void FlushDataTransfer() override;
		void CompleteFrameSubmissions() override;

		inline void ClearCachedAssets() override {  }
	private:
		DirectX12Core& m_Core;
		DirectX12ShaderCompiler m_ShaderCompiler;
	};
};


