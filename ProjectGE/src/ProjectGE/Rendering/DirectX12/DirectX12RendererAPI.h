#pragma once

#include "ProjectGE/Rendering/RendererAPI.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12Core.h"

namespace ProjectGE {
	class DirectX12RendererAPI : public RendererAPI
	{
	public:
		DirectX12RendererAPI();
		void SetClearColor(const glm::vec4& color) override;
		void Clear() override;

		void DrawIndexed(Ref<GeometryPack> geopack) override;

		void FinishRecording() override;
	private:
		DirectX12Core& m_Core;
	};
};


