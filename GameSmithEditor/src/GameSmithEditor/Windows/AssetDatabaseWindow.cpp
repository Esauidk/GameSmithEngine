#include "AssetDatabaseWindow.h"
#include "imgui.h"
#include "GameSmithEditor/Core/GameProject.h"
#include "GameSmithEditor/Icons/IconManager.h"
#include "GameSmithEditor/CustomWidgets/ReferenceInputWidget.h"

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

namespace GameSmithEditor {
	REGISTER_WINDOW_DEFAULT_CALLBACK(Windows_AssetDatabase, AssetDatabaseWindow);

	int AssetDatabaseWindow::s_Count = 0;

	AssetDatabaseWindow::AssetDatabaseWindow() : EditorWindow("AssetDatabaseWindow")
	{
		if (GameProject::IsLoaded()) {
			m_CurrentPath = std::filesystem::path(GameProject::GetAssetFolder());
		}
	}

	void AssetDatabaseWindow::OnAttach(const GameSmith::ApplicationSpecs& specs)
	{
		if (s_Count < 1) {
			IconManager::Init();
		}
		s_Count++;
	}

	void AssetDatabaseWindow::OnDetach()
	{
		s_Count--;
		if (s_Count < 1) {
			IconManager::Shutdown();
		}
	}

	void AssetDatabaseWindow::OnImGuiRender()
	{
		ImGui::Begin("Asset Database");
		static float padding = 16.0f;
		static float thumbailSize = 256;
		float cellSize = thumbailSize + padding;

		float windowWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(windowWidth / cellSize);
		if (columnCount < 1) {
			columnCount = 1;
		}

		if (GameProject::IsLoaded()) {
			if (m_CurrentPath == "") {
				m_CurrentPath = std::filesystem::path(GameProject::GetAssetFolder());
			}

			if (m_CurrentPath != std::filesystem::path(GameProject::GetAssetFolder())) {
				if (ImGui::Button("Back")) {
					m_CurrentPath = m_CurrentPath.parent_path();
				}
			}

			ImGui::Text(m_CurrentPath.filename().string().c_str());

			if (ImGui::BeginTable("Assets", columnCount)) {
				ImGuiStyle& style = ImGui::GetStyle();
				auto oldFramePadding = style.FramePadding;
				style.FramePadding = { 0,0 };
				for (const auto& dirEntry : recursive_directory_iterator(m_CurrentPath)) {
					std::string fileName = dirEntry.path().filename().string();
					if (fileName.ends_with(".meta")) {
						continue;
					}
					
					std::string path = dirEntry.path().string();
					bool isDir = dirEntry.is_directory();
					IconManager* iconManager = IconManager::GetInstance();
					ImTextureID image;
					if (isDir) {
						void* spot = iconManager->GetImGuiIcon("folder")->gpuSpot;
						image = reinterpret_cast<ImTextureID>(spot);
					}
					else {
						auto index = fileName.find_last_of(".");
						std::string fileExtention = fileName.substr(index + 1);

						auto filePtr = iconManager->GetImGuiIcon(fileExtention);
						if (filePtr == nullptr) {
							filePtr = iconManager->GetImGuiIcon("file");
						}

						image = reinterpret_cast<ImTextureID>(filePtr->gpuSpot);
					}

					if (ImGui::ImageButton(fileName.c_str(), image, { thumbailSize, thumbailSize })) {
						if (isDir) {
							m_CurrentPath = dirEntry.path();
						}
					}

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
						GameSmith::ResourceManager* rManager = GameSmith::ResourceManager::GetInstance();
						auto id = rManager->GetAssetID(path);
						ImGui::SetDragDropPayload(IMGUI_PAYLOAD_TYPE_ASSET_REF, &id, sizeof(GameSmith::ID));
						ImGui::EndDragDropSource();
					}

					ImGui::TextWrapped(fileName.c_str());
					ImGui::TableNextColumn();
				}
				style.FramePadding = oldFramePadding;
				ImGui::EndTable();
			}
		}

		ImGui::SliderFloat("Thumnail Size", &thumbailSize, 16, 512);
		ImGui::SliderFloat("Min Padding", &padding, 0, 32);
		ImGui::End();
	}
};

