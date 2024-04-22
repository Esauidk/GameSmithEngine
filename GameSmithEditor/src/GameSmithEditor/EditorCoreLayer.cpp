#include "EditorCoreLayer.h"

//TODO: Temporary, remove after demo
#include "SampleComponents/OrbitScript.h"

// TODO: Don't manually import each layer/window
#include "Windows/GameViewLayer.h"
#include "Windows/SimulationContentView.h"
#include "Windows/GameObjectDetails.h"
#include "imgui.h"

namespace GameSmithEditor {
	// TODO: Temporary, remove after demo
	static void RegisterSampleComponents() {
		auto registry = GameSmith::ComponentRegistry::GetInstance();
		registry->RegisterComponent("OrbitScript", [](GameSmith::GameObject* g, GameSmith::Transform* t) {return (GameSmith::Component*)new OrbitScript(g, t); });
	}

	EditorCoreLayer::EditorCoreLayer() : GameSmith::Layer("Editor Core"), m_App(GameSmith::Application::Get())
	{
		m_App.GetImGuiInstance()->SetDockspace(true);

		auto renderManager = GameSmith::RenderingManager::GetInstance();
		float color[4] = { 0.07f, 0.0f, 0.12f, 1.0f };
		m_EditorScreen = renderManager->GetRenderAPI()->CreateRenderTexture((float)m_App.GetWindow()->GetWidth(), (float)m_App.GetWindow()->GetHeight(), color);

		GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, m_EditorScreen.get()), false);
		renderManager->GetRenderAPI()->SetRenderTexture(m_EditorScreen, 0);
		renderManager->SetFrameTexture(m_EditorScreen);

		// TODO: Temporary, remove after demo
		RegisterSampleComponents();
	}

	void EditorCoreLayer::OnImGuiRender()
	{
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				ImGui::MenuItem("New Project");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows")) {
				if (ImGui::MenuItem("Game View")) {
					m_App.PushLayer(new GameViewLayer(m_EditorScreen));
				}

				if (ImGui::MenuItem("Simulation Content View")) {
					m_App.PushLayer(new SimulationContentView());
				}

				if (ImGui::MenuItem("Game Object Details")) {
					m_App.PushLayer(new GameObjectDetails());
				}

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
