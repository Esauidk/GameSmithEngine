#include "EditorCoreLayer.h"
#include "GameViewLayer.h"

namespace GameSmithEditor {
	EditorCoreLayer::EditorCoreLayer() : GameSmith::Layer("Editor Core")
	{
		auto& app = GameSmith::Application::Get();
		app.PushLayer(new GameViewLayer());
	}

	void EditorCoreLayer::OnImGuiRender()
	{
	}

};
