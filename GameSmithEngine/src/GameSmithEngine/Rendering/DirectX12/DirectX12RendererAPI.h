#pragma once

#include "GameSmithEngine/Rendering/DirectX12/DirectX12Core.h"
#include "GameSmithEngine/Rendering/DirectX12/DirectX12ShaderCompiler.h"
#include "GameSmithEngine/Rendering/DirectX12/Resources/DirectX12RingUploadHeap.h"
#include "GameSmithEngine/Rendering/RendererAPI.h"

namespace GameSmith {
	// DirectX12 implementation of the abstract RendererAPI. Creates and manages all
	// GPU resources (buffers, textures, shaders, PSOs) and issues draw calls through
	// the command context and state manager.
	class DirectX12RendererAPI : public RendererAPI
	{
	public:
		DirectX12RendererAPI();
		~DirectX12RendererAPI() override;
		inline API GetAPI() override { return API::DirectX12; }

		void DrawIndexed(UINT indecies, UINT instances) override;

		Ref<VertexBuffer> CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount) override;
		void SetVertexBuffer(Ref<VertexBuffer> vbuffer) override;
		Ref<IndexBuffer> CreateIndexBuffer(unsigned int* data, unsigned int indexCount) override;
		void SetIndexBuffer(Ref<IndexBuffer> ibuffer) override;
		Ref<Shader> LoadShader(std::string path) override;
		Ref<Shader> LoadShader(const char* byteCode, unsigned int length) override;
		Ref<const char> CompileShader(const Stages stage, const char* rawCode, const unsigned int length, const char* entryPt, ShaderIncludeCache* includeCache, unsigned int* outSize) override;

		Ref<ConstantBuffer> CreateConstantBuffer(UINT size, std::string name, UpdateFrequency frequency) override;
		Ref<ConstantBuffer> CreateConstantBuffer(UINT size, UpdateFrequency frequency) override;
		void SetConstantBuffer(Ref<ConstantBuffer> cbuffer, Stages stage, ShaderConstantType constantType) override;

		Ref<Texture2D> CreateTexture2D(char* data, UINT size) override;
		Ref<RenderTexture> CreateRenderTexture(unsigned int width, unsigned int height) override;
		Ref<RenderTexture> CreateRenderTexture(unsigned int width, unsigned int height, float* clearColor) override;

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
	private:
		DirectX12Core& m_Core;
		DirectX12ShaderCompiler m_ShaderCompiler;
		DirectX12RingUploadHeap m_UploadHeap;

		std::unordered_map<unsigned int, unsigned int> m_FrameIdToLifeIndex;
	};
};


