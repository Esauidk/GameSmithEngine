#include "SimulationContentView.h"
#include "imgui.h"
#include "GameSmithEditor/Utils/SystemCallUtils.h"
#include "GameSmithEditor/Core/GameProject.h"
#include "GameSmithEditor/CustomWidgets/ReferenceInputWidget.h"

namespace GameSmithEditor {
	GameSmith::Connection<GameSmith::GameObject> SimulationContentView::m_SelectedObjected = GameSmith::Connection<GameSmith::GameObject>();

	REGISTER_WINDOW_DEFAULT_CALLBACK(Windows_SimulationContentView, SimulationContentView);

	void SimulationContentView::OnImGuiRender()
	{
		ImGui::SetNextWindowSizeConstraints(ImVec2(20, 20), ImVec2(FLT_MAX, FLT_MAX));
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

			std::string file = CreateFileDialog(
				"C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone",
				GameSmith::GameChunkAsset::GetStaticFileType(),
				GameSmith::GameChunkAsset::GetStaticFileExtension()
			);
			assetManager->WriteResource(chunk, file);
			
		}

		if (ImGui::Button("Load Chunk")) {
			std::string file = PickFileDialog(
				"C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone", 
				GameSmith::GameChunkAsset::GetStaticFileType(), 
				GameSmith::GameChunkAsset::GetStaticFileExtension()
			);

			auto assetManager = GameSmith::ResourceManager::GetInstance();
			auto ID = assetManager->GetAssetID(file);
			auto chunkManager = GameSmith::ChunkManager::GetInstance();
			chunkManager->LoadChunk(ID);
		}

		if (ImGui::BeginListBox("Objects", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()))) {
			for (int i = 0; i < m_NamesStd.size(); i++) {
				bool selected = (m_Selection == i);
				if (ImGui::Selectable(m_NamesStd[i].c_str(), selected)) {
					auto gm = GameSmith::GameObjectManager::GetInstance();
					m_SelectedObjected = gm->FindGameObject(m_NamesStd[i]);
					m_Selection = i;
				}

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
					auto gm = GameSmith::GameObjectManager::GetInstance();
					auto g = gm->FindGameObject(m_NamesStd[i]);
					ImGui::SetDragDropPayload(IMGUI_PAYLOAD_TYPE_GAMEOBJECT_REF, &g, sizeof(GameSmith::Connection<GameSmith::GameObject>));
					ImGui::EndDragDropSource();
				}

				if (selected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndListBox();
		}
		
		ImGui::End();
	}

	void SimulationContentView::OnUpdate()
	{
		auto gameObjectMang = GameSmith::GameObjectManager::GetInstance();
		m_NamesStd.clear();
		gameObjectMang->GetGameObjectNames(&m_NamesStd);
	}
};

