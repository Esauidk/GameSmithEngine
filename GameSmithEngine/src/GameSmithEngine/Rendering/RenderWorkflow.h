#pragma once
#include "GameSmithEngine/Rendering/RendererAPI.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/MaterialSystem/Material.h"

namespace GameSmith {
	class RenderWorkflow {
	public:
		RenderWorkflow(RendererAPI* api) : m_APIInstance(api) {}
		virtual void Submit(Ref<VertexBuffer> vBuff, Ref<IndexBuffer> iBuff, Ref<Material> mat) const = 0;
	protected:
		RendererAPI* m_APIInstance;
	};
};
