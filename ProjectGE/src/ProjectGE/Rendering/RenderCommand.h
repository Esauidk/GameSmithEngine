#pragma once

#include "RendererAPI.h"
namespace ProjectGE {
	class RenderCommand {
	public:
		inline static void Clear() {
			s_RendererAPI->Clear();
		}

		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		};

		inline static void DrawIndexed(VertexBuffer* vBuffer, IndexBuffer* iBuffer) {
			s_RendererAPI->DrawIndexed(vBuffer, iBuffer);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
};
