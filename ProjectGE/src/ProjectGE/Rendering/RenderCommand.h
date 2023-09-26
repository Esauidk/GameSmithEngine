#pragma once

#include "RendererAPI.h"
namespace ProjectGE {
	// A class that can divert commands to the Engine's RendererAPI according to what Render API is being used
	class RenderCommand {
	public:
		inline static void Clear() {
			s_RendererAPI->Clear();
		}

		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		};

		inline static void DrawIndexed(UINT indecies, UINT instances) {
			s_RendererAPI->DrawIndexed(indecies, instances);
		}

		inline static Ref<VertexBuffer> CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount) {
			return s_RendererAPI->CreateVertexBuffer(data, vertexByteSize, vertexCount);
		}

		inline static void SetVertexBuffer(Ref<VertexBuffer> vBuffer) {
			s_RendererAPI->SetVertexBuffer(vBuffer);
		}
		
		inline static Ref<IndexBuffer> CreateIndexBuffer(unsigned short* data, unsigned int indexCount) {
			return s_RendererAPI->CreateIndexBuffer(data, indexCount);
		}
		
		inline static void SetIndexBuffer(Ref<IndexBuffer> iBuffer) {
			s_RendererAPI->SetIndexBuffer(iBuffer);
		}

		inline static Ref<ConstantBuffer> CreateConstantBuffer(UINT size) {
			return s_RendererAPI->CreateConstantBuffer(size);
		}

		inline static void SetConstantBuffer(Ref<ConstantBuffer> cbuffer, Stages stage, ShaderConstantType constantType) {
			s_RendererAPI->SetConstantBuffer(cbuffer, stage, constantType);
		}

		inline static Ref<Texture2D> CreateTexture2D(std::string texturePath) {
			return s_RendererAPI->CreateTexture2D(texturePath);
		}

		inline static void SetTexture2D(Ref<Texture2D> tex, Stages stage) {
			return s_RendererAPI->SetTexture2D(tex, stage);
		}

		inline static Ref<Sampler> CreateSampler(FilterType img, PaddingMethod padMode) {
			return s_RendererAPI->CreateSampler(img, padMode);
		}

		inline static void SetSampler(Ref<Sampler> sampler, Stages stage) {
			s_RendererAPI->SetSampler(sampler, stage);
		}

		inline static Ref<Shader> LoadShader(std::string path) {
			return s_RendererAPI->LoadShader(path);
		}

		inline static void SetTopology(TopologyType type) {
			s_RendererAPI->SetTopology(type);
		}

		inline static void SubmitRecording() {
			s_RendererAPI->SubmitRecording();
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
};
