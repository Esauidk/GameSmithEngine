#include "AssetDatabaseWindow.h"
#include "GameSmithEditor/Core/GameProject.h"
#include "GameSmithEditor/CustomWidgets/ReferenceInputWidget.h"
#include "GameSmithEditor/CustomWidgets/SelectableImage.h"
#include "GameSmithEditor/Icons/IconManager.h"
#include "GameSmithEditor/Utils/SystemCallUtils.h"
#include "GameSmithEditor/Windows/PrivateWindows/AssetInspector.h"
#include "imgui.h"

using directory_iterator = std::filesystem::directory_iterator;

namespace GameSmithEditor {
	REGISTER_WINDOW_DEFAULT_CALLBACK(Windows_AssetDatabase, AssetDatabaseWindow);

	int AssetDatabaseWindow::s_Count = 0;

	AssetDatabaseWindow::AssetDatabaseWindow() : EditorWindow("AssetDatabaseWindow")
	{
		if (GameProject::IsLoaded()) {
			m_CurrentPath = std::filesystem::path(GameProject::GetAssetFolder());
		}

		m_RegisteredAssets = GameSmith::AssetFactory::GetInstance()->GetLabels();
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
		if (ImGui::Begin("Asset Database", &m_Open, ImGuiWindowFlags_MenuBar)) {
			static float padding = 16.0f;
			static float thumbailSize = 256;
			float cellSize = thumbailSize + padding;

			float windowWidth = ImGui::GetContentRegionAvail().x;
			int columnCount = (int)(windowWidth / cellSize);
			if (columnCount < 1) {
				columnCount = 1;
			}
			
			auto assetManager = GameSmith::AssetManager::GetInstance();
			const auto& resourceRegistry = assetManager->GetAssetDirectories();

			bool directoryChanged = false;
			if (ImGui::BeginMenuBar()) {
				for (const auto& [key, value] : resourceRegistry) {
					if (ImGui::MenuItem(key.c_str(), nullptr, !key.compare(m_SelectedDirectory))) {
						m_SelectedDirectory = key;
						directoryChanged = true;
					}
				}
				ImGui::EndMenuBar();
			}

			if (resourceRegistry.contains(m_SelectedDirectory)) {
				auto rootPath = std::filesystem::path(resourceRegistry.at(m_SelectedDirectory));
				if (directoryChanged) {
					m_CurrentPath = rootPath;
				}

				if (m_CurrentPath != rootPath) {
					if (ImGui::Button("Back")) {
						m_CurrentPath = m_CurrentPath.parent_path();
					}
				}

				ImGui::SameLine();

				if (ImGui::Button("+")) {
					ImGui::OpenPopup("AssetDatabaseModMenu");
				}

				if (ImGui::BeginPopup("AssetDatabaseModMenu")) {
					if (ImGui::Button("ImportAsset")) {
						std::string filePath;
						FileSearchCriteria criteria;
						criteria.filePath = resourceRegistry.at(m_SelectedDirectory);
						if (PickFileDialog(criteria, &filePath)) {
							assetManager->ImportResource(filePath, m_SelectedDirectory);
						}
						
					}

					if (ImGui::Button("CreateAsset")) {
						ImGui::OpenPopup("AssetDatabaseCreateAssetMenu");
					}

					if (ImGui::BeginPopup("AssetDatabaseCreateAssetMenu")) {
						for (const auto& entry : m_RegisteredAssets) {
							const std::string menuLabel = std::format("{1} ({0})", entry.first, entry.second);
							if (ImGui::MenuItem(menuLabel.c_str())) {

								const std::string fileName = std::format("New {1}{0}", entry.first, entry.second);
								const std::string path = m_CurrentPath.string();
								assetManager->CreateResource(fileName, path);
							}
						}
						ImGui::EndPopup();
					}

					ImGui::EndPopup();
				}

				

				ImGui::Separator();

				if (ImGui::BeginTable("Assets", columnCount)) {
					ImGui::TableNextColumn();
					ImGuiStyle& style = ImGui::GetStyle();
					auto oldFramePadding = style.FramePadding;
					style.FramePadding = { 0,0 };
					for (const auto& dirEntry : directory_iterator(m_CurrentPath)) {
						std::string fileName = dirEntry.path().filename().string();
						if (fileName.ends_with(".meta")) {
							continue;
						}

						std::string path = dirEntry.path().string();
						bool isDir = dirEntry.is_directory();
						IconManager* iconManager = IconManager::GetInstance();
						ImTextureID image;
						if (isDir) {
							const GameSmith::ImGuiTextureSpace* icon = iconManager->GetImGuiIcon("folder");
							if (icon == nullptr) {
								GE_APP_ERROR("No folder icon found in the icon manager, skipping rendering...");
								continue;
							}
							void* spot = icon->gpuSpot;
							image = reinterpret_cast<ImTextureID>(spot);
						}
						else {
							auto index = fileName.find_last_of(".");
							std::string fileExtention = fileName.substr(index + 1);

							auto filePtr = iconManager->GetImGuiIcon(fileExtention);
							if (filePtr == nullptr) {
								filePtr = iconManager->GetImGuiIcon("file");
								if (filePtr == nullptr) {
									GE_APP_ERROR("No file icon found in the icon manager, skipping rendering...");
									continue;
								}
							}

							image = reinterpret_cast<ImTextureID>(filePtr->gpuSpot);
							
						}

						ImTextureRef imageRef = ImTextureRef(image);
						ImVec2 imageSize = { thumbailSize, thumbailSize };
						bool selected = m_SelectedAsset == fileName;
						if (SelectableImage(fileName, selected, imageRef, imageSize)) {
							m_SelectedAsset = fileName;

							if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
								if (isDir) {
									m_CurrentPath = dirEntry.path();
									m_SelectedAsset = "";
								}
								else {
									auto id = assetManager->GetAssetID(path);
									auto asset = assetManager->GetResource(id);
									auto inspector = new AssetInspector(asset);
									EditorCoreLayer::GetInstance()->AppendEditorWindow(inspector);
								}
							}	
						}

						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
							auto id = assetManager->GetAssetID(path);
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
		}
		ImGui::End();
	}

	void AssetDatabaseWindow::OnUpdate(float dt)
	{
		if (!m_Open) {
			CloseWindow();
		}
	}
};

