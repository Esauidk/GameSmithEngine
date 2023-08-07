#pragma once

#include <glm/glm.hpp>

#include "RenderAgnostics/BindableResources/VertexBuffer.h"
#include "RenderAgnostics/BindableResources/IndexBuffer.h"

namespace ProjectGE {
	class RendererAPI {
	public:
		enum class API {
			None = 0, DirectX12 = 1
		};

		virtual void Clear() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;

		virtual void DrawIndexed(VertexBuffer* vBuffer, IndexBuffer* iBuffer) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
};
