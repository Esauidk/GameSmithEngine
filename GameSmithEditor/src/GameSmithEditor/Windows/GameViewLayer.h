#pragma once
// TODO: Fix project settings to have game editor folder in include directory
#include "GameSmithEditor/EngineDependenciesOnly.h"


namespace GameSmithEditor {
	class GameViewLayer : public GameSmith::Layer {
	public:
		GameViewLayer(GameSmith::Ref<GameSmith::RenderTexture> screenTex);
		void OnImGuiRender() override;
		void OnUpdate() override;
	private:
		GameSmith::Ref<GameSmith::RenderTexture> m_GameViewTexture;
		GameSmith::Ref<GameSmith::RenderTexture> m_GameScreenTexture;
		GameSmith::ImGuiTextureSpace m_GameViewHandle;
	};
};

