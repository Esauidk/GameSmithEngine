#pragma once

#include "ProjectGE/Rendering/RendererAPI.h"

namespace ProjectGE {
	class DirectX12RendererAPI : public RendererAPI
	{
	public:
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(GeometryPack* geopack) override;

		void FinishRecording() override;
	};
};


