#pragma once
#include "EngineDependenciesOnly.h"

namespace GameSmithEditor {
	class GameViewLayer : public GameSmith::Layer {
	public:
		GameViewLayer();
		void OnImGuiRender() override;
		void OnUpdate() override;
	private:
		GameSmith::Ref<GameSmith::RenderTexture> m_GameViewTexture;

		// TODO: Remove as this is just for testing
		GameSmith::Ref<GameSmith::RenderTexture> m_RenderTex;
	};
};

