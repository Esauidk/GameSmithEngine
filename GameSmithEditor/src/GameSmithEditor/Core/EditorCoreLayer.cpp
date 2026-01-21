#include "EditorCoreLayer.h"
#include "GameSmithEditor/Core/EditorMenu.h"
#include "GameSmithEditor/Core/GameProject.h"
#include "GameSmithEditor/Windows/AccessibleWindows.h"
#include "imgui.h"

namespace GameSmithEditor {
	EditorCoreLayer* EditorCoreLayer::s_Instance = nullptr;

	EditorCoreLayer::EditorCoreLayer() : GameSmith::Layer("Editor Core"), m_App(GameSmith::Application::Get())
	{
		if (s_Instance == nullptr) {
			s_Instance = this;

			auto renderManager = GameSmith::RenderingManager::GetInstance();
			float color[4] = { 0.07f, 0.0f, 0.12f, 1.0f };
			m_EditorScreen = renderManager->GetRenderAPI()->CreateRenderTexture(m_App.GetWindow()->GetWidth(), m_App.GetWindow()->GetHeight(), color);

			GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, m_EditorScreen.get()), false);
			renderManager->GetRenderAPI()->SetRenderTexture(m_EditorScreen, 0);
			renderManager->SetFrameTexture(m_EditorScreen);

			GameViewLayer::SetScreenTexture(m_EditorScreen);
		}
	}

	void EditorCoreLayer::OnAttach(const GameSmith::ApplicationSpecs& specs)
	{
		auto ImGuiInstance = m_App.GetImGuiInstance();

		ImGuiInstance->GetImGuiContext();
		ImGui::SetCurrentContext(ImGuiInstance->GetImGuiContext());

		ImGuiInstance->SetDockspace(true);
	}

	void EditorCoreLayer::OnImGuiRender()
	{
		if (ImGui::BeginMainMenuBar()) {
			std::vector<std::string> paths;
			MenuExecutor::GetMenuPaths(&paths);

			for (auto& menuItem : paths) {
				std::vector<std::string> seperates;
				std::istringstream ss(menuItem);

				std::string s;
				while (std::getline(ss, s, '_')) {
					seperates.push_back(s);
				}

				int size = (int)seperates.size();
				int menusCreated = 0;
				for (int i = 0; i < size; i++) {
					std::string& path = seperates[i];
					if (i < size - 1) {
						if (!ImGui::BeginMenu(path.c_str())) {
							break;
						}
						menusCreated++;
					}
					else {
						if (ImGui::MenuItem(path.c_str())) {
							MenuExecutor::ExecuteCallBack(menuItem);
						}
					}
				}

				for (int i = 0; i < menusCreated; i++) {
					ImGui::EndMenu();
				}
			}
			
			ImGui::EndMainMenuBar();
		}
		
	}

	void EditorCoreLayer::OnUpdate(float dt)
	{
		if (GameProject::IsLoaded()) {
			std::string prjName = GameProject::GetProjectName();
			std::string newWindowTitle = std::format("{} - {}", prjName, EDITOR_WINDOW_TITLE);

			m_App.GetWindow()->SetTitle(newWindowTitle);
		}
		else {
			m_App.GetWindow()->SetTitle(EDITOR_WINDOW_TITLE);
		}

		auto renderManager = GameSmith::RenderingManager::GetInstance();
		renderManager->SetForClear(m_EditorScreen);
	}

	void EditorWindow::CloseWindow()
	{
		EditorWindowClose closeEvent(this);
		m_CloseDispatcher.Dispatch(closeEvent);
	}
};
