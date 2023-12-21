#pragma once
#include "GameSmithEngine/Rendering/RenderWorkflow.h"

namespace GameSmith {
	class ForwardRender : public RenderWorkflow {
	public:
		ForwardRender(RendererAPI* api) : RenderWorkflow(api) {}
		virtual void Submit(Ref<VertexBuffer> vBuff, Ref<IndexBuffer> iBuff, Ref<Material> mat);
	};
};
