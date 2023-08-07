#pragma once

#include "RenderCommand.h"

namespace ProjectGE {
	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();
		static void Submit(VertexBuffer* vBuffer, IndexBuffer* iBuffer);
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };
	};
};


