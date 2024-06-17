#pragma once
#include "GameSmithEditor/EngineDependenciesOnly.h"
#include "GameSmithEditor/Windows/Utils/WindowRegistrator.h"


namespace GameSmithEditor {
	class GameViewLayer : public GameSmith::Layer {
	public:
		GameViewLayer();
		inline void SetScreenTexture(GameSmith::Ref<GameSmith::RenderTexture> screenTex) { m_GameScreenTexture = screenTex; };
		void OnImGuiRender() override;
		void OnUpdate() override;
	private:
		GameSmith::Ref<GameSmith::RenderTexture> m_GameViewTexture;
		GameSmith::Ref<GameSmith::RenderTexture> m_GameScreenTexture;
		GameSmith::ImGuiTextureSpace m_GameViewHandle;
		static WindowRegistrator<GameViewLayer> s_Registrator;
	};
};

