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

		inline static void DrawIndexed(Ref<GeometryPack> geopack) {
			s_RendererAPI->DrawIndexed(geopack);
		}

		inline static void FinishedRecording() {
			s_RendererAPI->FinishRecording();
		}

		inline static Ref<VertexBuffer> CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount) {
			return s_RendererAPI->CreateVertexBuffer(data, vertexByteSize, vertexCount);
		}

		inline static void SetVertexBuffer(Ref<VertexBuffer> vBuffer) {
			s_RendererAPI->SetVertexBuffer(vBuffer);
		}
		
		inline static Ref<IndexBuffer> CreateIndexBuffer(unsigned int* data, unsigned int indexCount) {
			return s_RendererAPI->CreateIndexBuffer(data, indexCount);
		}
		
		inline static void SetIndexBuffer(Ref<IndexBuffer> iBuffer) {
			s_RendererAPI->SetIndexBuffer(iBuffer);
		}

		inline static Ref<Shader> LoadShader() {
			return s_RendererAPI->LoadShader();
		}

		inline static void SubmitRecording() {
			s_RendererAPI->SubmitRecording();
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
};
