#include "SimulationContentView.h"
#include "imgui.h"
#include "GameSmithEditor/EditorCoreLayer.h"

namespace GameSmithEditor {
	GameSmith::Connection<GameSmith::GameObject> SimulationContentView::m_SelectedObjected = GameSmith::Connection<GameSmith::GameObject>();
	WindowRegistrator<SimulationContentView> SimulationContentView::s_Registrator("Simulation Content View");

	void SimulationContentView::OnImGuiRender()
	{
		ImGui::Begin("Simulation Content View");
		if (ImGui::Button("Create GameObject")) {
			auto gm = GameSmith::GameObjectManager::GetInstance();
			auto object = gm->CreateGameObject();
			auto chunkManager = GameSmith::ChunkManager::GetInstance();
			chunkManager->GetCurrentMainChunk().lock()->AddObjectToChunk(object);
			m_NamesStd.push_back(object.lock()->GetName());
		}

		ImGui::SameLine();

		if (ImGui::Button("Save Chunk")) {
			auto assetManager = GameSmith::ResourceManager::GetInstance();
			auto chunkManager = GameSmith::ChunkManager::GetInstance();
			auto chunk = chunkManager->GetCurrentMainChunk().lock();
			assetManager->WriteResource(chunk, "C:\\Users\\esaus\\Desktop\\TestProjectDir\\Assets\\Test2.chunk");
			
		}

		if (ImGui::Button("Load Chunk")) {
			auto assetManager = GameSmith::ResourceManager::GetInstance();
			auto chunkManager = GameSmith::ChunkManager::GetInstance();
			chunkManager->LoadChunk(GameSmith::ID(147063809, 30548, 17519, 1851296716170764188));

		}

		ImGui::BeginListBox("Objects", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
		for (int i = 0; i < m_NamesStd.size(); i++) {
			bool selected = (m_Selection == i);
			if (ImGui::Selectable(m_NamesStd[i].c_str(), selected)) {
				auto gm = GameSmith::GameObjectManager::GetInstance();
				m_SelectedObjected = gm->FindGameObject(m_NamesStd[i]);
				m_Selection = i;
			}

			if (selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
		ImGui::End();
	}

	void SimulationContentView::OnUpdate()
	{
		auto gameObjectMang = GameSmith::GameObjectManager::GetInstance();
		m_NamesStd.clear();
		gameObjectMang->GetGameObjectNames(&m_NamesStd);
	}
};

