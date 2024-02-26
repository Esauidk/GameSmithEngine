#pragma once
#include "EngineDependenciesOnly.h"

namespace GameSmithEditor {
	class EditorCoreLayer : public GameSmith::Layer
	{
	public:
		EditorCoreLayer();
		void OnImGuiRender() override;
		void OnUpdate() override;
	private:
		GameSmith::Application& m_App;
		GameSmith::Ref<GameSmith::RenderTexture> m_EditorScreen;
	};
};


