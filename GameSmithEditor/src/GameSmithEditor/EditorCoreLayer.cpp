#include "EditorCoreLayer.h"
#include "Windows/GameViewLayer.h"
#include "imgui.h"

namespace GameSmithEditor {
	EditorCoreLayer::EditorCoreLayer() : GameSmith::Layer("Editor Core")
	{
		auto& app = GameSmith::Application::Get();
		app.GetImGuiInstance()->SetDockspace(true);

		auto renderManager = GameSmith::RenderingManager::GetInstance();
		float color[4] = { 0.07f, 0.0f, 0.12f, 1.0f };
		m_EditorScreen = renderManager->GetRenderAPI()->CreateRenderTexture((float)app.GetWindow()->GetWidth(), (float)app.GetWindow()->GetHeight(), color);

		GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, m_EditorScreen.get()), false);
		renderManager->GetRenderAPI()->SetRenderTexture(m_EditorScreen, 0);
		renderManager->SetFrameTexture(m_EditorScreen);

		app.PushLayer(new GameViewLayer(m_EditorScreen));
	}

	void EditorCoreLayer::OnImGuiRender()
	{
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				ImGui::MenuItem("New Project");
				ImGui::EndMenu();
			}
			
			ImGui::EndMainMenuBar();
		}
		
	}

	void EditorCoreLayer::OnUpdate()
	{
		auto renderManager = GameSmith::RenderingManager::GetInstance();
		renderManager->SetForClear(m_EditorScreen);
	}

};
