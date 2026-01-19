#include "ContentView.h"
#include "imgui.h"

#include "GameSmithEditor/Core/GameProject.h"
#include "GameSmithEditor/CustomWidgets/ReferenceInputWidget.h"
#include "GameSmithEditor/Utils/SystemCallUtils.h"
#include "GameSmithEditor/Windows/Inspector.h"

namespace GameSmithEditor {
	REGISTER_WINDOW_DEFAULT_CALLBACK(Windows_ContentView, ContentView);

	void ContentView::OnImGuiRender()
	{
		ImGui::SetNextWindowSizeConstraints(ImVec2(20, 20), ImVec2(FLT_MAX, FLT_MAX));
		if (ImGui::Begin("Content View", &m_Open)) {
			auto chunkManager = GameSmith::ChunkManager::GetInstance();
			if (ImGui::Button("Create GameObject")) {
				auto gm = GameSmith::GameObjectManager::GetInstance();
				auto object = gm->CreateGameObject();
				chunkManager->GetCurrentMainChunk().lock()->AddObjectToChunk(object);
				m_NamesStd.push_back(object.lock()->GetName());
			}

			ImGui::SameLine();

			if (ImGui::Button("Save Chunk")) {
				auto assetManager = GameSmith::AssetManager::GetInstance();
				auto chunk = chunkManager->GetCurrentMainChunk().lock();

				std::string location = std::format(
					"{0}\\{1}.{2}",
					GameProject::GetAssetFolder(),
					chunk->GetChunkName(),
					GameSmith::GameChunkAsset::GetStaticFileExtension()
				);
				assetManager->WriteResource(chunk, location);

			}

			if (ImGui::Button("Load Chunk")) {
				std::string file = PickFileDialog(
					"C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone",
					GameSmith::GameChunkAsset::GetStaticFileType(),
					GameSmith::GameChunkAsset::GetStaticFileExtension()
				);

				auto assetManager = GameSmith::AssetManager::GetInstance();
				auto ID = assetManager->GetAssetID(file);
				auto chunkManager = GameSmith::ChunkManager::GetInstance();
				chunkManager->LoadChunk(ID);
			}

			std::string chunkName = chunkManager->GetCurrentMainChunk().lock()->GetChunkName();
			std::string labelName = std::format("##{}", chunkName);
			if (ImGui::CollapsingHeader(chunkName.c_str())) {
				if (ImGui::BeginListBox(labelName.c_str(), ImVec2(ImGui::GetWindowWidth() - 25, ImGui::GetWindowHeight() / 2))) {
					for (int i = 0; i < m_NamesStd.size(); i++) {
						bool selected = (m_Selection == i);
						if (ImGui::Selectable(m_NamesStd[i].c_str(), selected)) {
							auto gm = GameSmith::GameObjectManager::GetInstance();
							Inspector::SetInspectedGameObject(gm->FindGameObject(m_NamesStd[i]));
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
			}
			
		}
		
		ImGui::End();
	}

	void ContentView::OnUpdate(float dt)
	{
		m_NamesStd.clear();
		auto chunkManager = GameSmith::ChunkManager::GetInstance();
		chunkManager->GetCurrentMainChunk().lock()->GetChunkGameObjectNames(&m_NamesStd);

		if (!m_Open) {
			CloseWindow();
		}
	}
};

