#pragma once

#include "ProjectGE/Rendering/RendererContext.h"

#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/VertexBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/IndexBuffer.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ShaderArguementDefiner.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Shader.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/PipelineStateObject.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Topology.h"
#include "ProjectGE/Rendering/RenderAgnostics/BasicStructs.h"

namespace ProjectGE {
	class TriangleDemo {
	public:
		TriangleDemo();
		virtual void Draw(RendererContext* renderer) = 0;
		static TriangleDemo* Create();
	protected:
		std::unique_ptr<PipelineStateObject> m_State;
		std::unique_ptr<ShaderArguementDefiner> m_Root;
		std::unique_ptr<Topology> m_Topo;
		std::unique_ptr<Shader> m_VS;
		std::unique_ptr<Shader> m_PS;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		int m_IndexCount;
	};
}
