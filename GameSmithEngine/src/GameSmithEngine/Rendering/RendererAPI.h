#pragma once

#include <glm/glm.hpp>

#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/VertexBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/IndexBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/ConstantBuffer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Texture.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Shader.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Sampler.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/PipelineStateInitializer.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/PipelineStateObject.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"
#include "GameSmithEngine/Core/Core.h"

namespace GameSmith {
	// INTERFACE
	// A object that can perform commands using the specified Render API
	class RendererAPI {
	public:
		enum class API {
			None = 0, DirectX12 = 1
		};

		virtual ~RendererAPI() {};
		
		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void DrawIndexed(UINT indecies, UINT instances) = 0;

		virtual Ref<VertexBuffer> CreateVertexBuffer(BYTE* data, int vertexByteSize, int vertexCount) = 0;
		virtual void SetVertexBuffer(Ref<VertexBuffer> vbuffer) = 0;
		virtual Ref<IndexBuffer> CreateIndexBuffer(unsigned int* data, unsigned int indexCount) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer> ibuffer) = 0;
		virtual Ref<Shader> LoadShader(std::string path) = 0;
		virtual Ref<Shader> LoadShader(char* byteCode, unsigned int length) = 0;

		virtual Ref<ConstantBuffer> CreateConstantBuffer(UINT size, std::string name) = 0;
		virtual Ref<ConstantBuffer> CreateConstantBuffer(UINT size) = 0;
		virtual void SetConstantBuffer(Ref<ConstantBuffer> cbuffer, Stages stage, ShaderConstantType constantType) = 0;

		virtual Ref<Texture2D> CreateTexture2D(char* data, UINT size) = 0;
		virtual Ref<RenderTexture> CreateRenderTexture(unsigned int width, unsigned int height) = 0;
		virtual Ref<RenderTexture> CreateRenderTexture(unsigned int width, unsigned int height, float* clearColor) = 0;

		virtual void SetRenderTexture(Ref<RenderTexture> rt, UINT index) = 0;
		virtual void SetTexture2D(Ref<Texture2D> tex, UINT slot, Stages stage) = 0;

		virtual Ref<Sampler> CreateSampler(FilterType img, PaddingMethod padMode) = 0;
		virtual void SetSampler(Ref<Sampler> sampler, Stages stage) = 0;

		virtual void SetTopology(TopologyType& type, bool tesselation) = 0;

		virtual Ref<GraphicsPipelineStateObject> CreateGraphicsPipelineState(PipelineStateInitializer& init) = 0;
		virtual void SetGraphicsPipelineState(Ref<GraphicsPipelineStateObject> pso) = 0;

		virtual void SubmitRecording() = 0;
		virtual void CompleteFrameSubmissions() = 0;
		virtual void ClearCachedAssets() = 0;

		virtual API GetAPI() = 0;
	};
};
