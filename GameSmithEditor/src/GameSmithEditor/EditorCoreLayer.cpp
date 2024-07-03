#include "EditorCoreLayer.h"

//TODO: Temporary, remove after demo
#include "SampleComponents/OrbitScript.h"
#include "SampleComponents/BurstOrbitScript.h"

// TODO: Don't manually import each layer/window
#include "GameSmithEditor/Windows/AccessibleWindows.h"

#include "imgui.h"
#include "GameSmithEditor/Core/GameProject.h"

#include "GameSmithEditor/Utils/SystemCallUtils.h"

namespace GameSmithEditor {
	EditorCoreLayer* EditorCoreLayer::s_Instance = nullptr;

	std::unordered_map<std::string, std::function<GameSmith::Layer* ()>> EditorCoreLayer::m_WindowRegistry;

	EditorCoreLayer::EditorCoreLayer() : GameSmith::Layer("Editor Core"), m_App(GameSmith::Application::Get())
	{
		if (s_Instance == nullptr) {
			s_Instance = this;

			m_App.GetImGuiInstance()->SetDockspace(true);

			auto renderManager = GameSmith::RenderingManager::GetInstance();
			float color[4] = { 0.07f, 0.0f, 0.12f, 1.0f };
			m_EditorScreen = renderManager->GetRenderAPI()->CreateRenderTexture((float)m_App.GetWindow()->GetWidth(), (float)m_App.GetWindow()->GetHeight(), color);

			GameSmith::RegisterEvent<GameSmith::WindowResizeEvent>(&GameSmith::Window::s_Resized, GE_BIND_EVENT_FN(GameSmith::RenderTexture::WindowResized, m_EditorScreen.get()), false);
			renderManager->GetRenderAPI()->SetRenderTexture(m_EditorScreen, 0);
			renderManager->SetFrameTexture(m_EditorScreen);

			// TODO: Temporary, remove after testing
			auto chunkManager = GameSmith::ChunkManager::GetInstance();
			auto chunk = GameSmith::Ref<GameSmith::GameChunk>(new GameSmith::GameChunk());
			chunkManager->LoadChunk(chunk->GetId(), chunk);

			if (GameProject::IsLoaded()) {
			
			}
			else {
				
			}
		}
		
	}

	void EditorCoreLayer::OnImGuiRender()
	{
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New Project")) {
					std::string rootFolder = PickFolderDialog("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone");
					GameSmithEditor::GameProject::CreateProject("TestProjectDir", rootFolder);
				}

				if (ImGui::MenuItem("Load Project")) {
					std::string rootFolder = PickFolderDialog("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone");
					GameSmithEditor::GameProject::LoadProject(rootFolder);
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows")) {
				for (auto& windowEntry : m_WindowRegistry) {
					if (ImGui::MenuItem(windowEntry.first.c_str())) {
						m_App.PushLayer(windowEntry.second());
					}
				}
				if (ImGui::MenuItem("Game View")) {
					auto gmView = new GameViewLayer();
					gmView->SetScreenTexture(m_EditorScreen);
					m_App.PushLayer(gmView);
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

	void EditorCoreLayer::RegisterWindow(std::string windowName, std::function<GameSmith::Layer* ()> windowCreator)
	{
		if (m_WindowRegistry.contains(windowName)) {
			GE_APP_WARN(std::format("There already exists a definition for the {0} window, overriting...", windowName));
		}
		
		m_WindowRegistry.insert({ windowName, windowCreator });
		
	}

};
