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

		inline static void DrawIndexed(GeometryPack* geopack) {
			s_RendererAPI->DrawIndexed(geopack);
		}

		inline static void FinishedRecording() {
			s_RendererAPI->FinishRecording();
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
};
