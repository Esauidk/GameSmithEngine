#pragma once

#include <glm/glm.hpp>

#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/GeometryPack.h"

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

		virtual void DrawIndexed(std::shared_ptr<GeometryPack> geopack) = 0;

		virtual void FinishRecording() = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};
};
