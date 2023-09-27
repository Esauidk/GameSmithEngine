#pragma once

#include <glm/glm.hpp>

#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/GeometryPack.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/ConstantBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Shader.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Sampler.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/PipelineStateInitializer.h"
#include "ProjectGE/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"
#include "ProjectGE/Core/Core.h"

namespace ProjectGE {
	// INTERFACE
	// A object that can perform commands using the specified Render API
	class RendererAPI {
	public:
		enum class API {
			None = 0, DirectX12 = 1
		};
		
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void DrawIndexed(UINT indecies, UINT instances) = 0;

		virtual Ref<VertexBuffer> CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount) = 0;
		virtual void SetVertexBuffer(Ref<VertexBuffer> vbuffer) = 0;
		virtual Ref<IndexBuffer> CreateIndexBuffer(unsigned short* data, unsigned int indexCount) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer> ibuffer) = 0;
		virtual Ref<Shader> LoadShader(std::string path) = 0;

		virtual Ref<ConstantBuffer> CreateConstantBuffer(UINT size) = 0;
		virtual void SetConstantBuffer(Ref<ConstantBuffer> cbuffer, Stages stage, ShaderConstantType constantType) = 0;

		virtual Ref<Texture2D> CreateTexture2D(std::string& texturePath) = 0;
		virtual void SetTexture2D(Ref<Texture2D> tex, Stages stage) = 0;

		virtual Ref<Sampler> CreateSampler(FilterType img, PaddingMethod padMode) = 0;
		virtual void SetSampler(Ref<Sampler> sampler, Stages stage) = 0;

		virtual void SetTopology(TopologyType& type) = 0;

		virtual void UpdatePipeline(PipelineStateInitializer& init) = 0;
		virtual void SubmitRecording() = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
};
