#include "AssetInspector.h"
#include "imgui.h"

#include "GameSmithEditor/Utils/ExposedVariableWidgetConverter.h"
#include "GameSmithEditor/CustomWidgets/ReferenceInputWidget.h"

#include <filesystem>

namespace fs = std::filesystem;

namespace GameSmithEditor {
	AssetInspector::AssetInspector(GameSmith::Ref<GameSmith::IAsset> asset) : EditorWindow("AssetInspector"), m_InspectedAsset(asset)
	{
		m_InspectedAsset->GenerateVariableEntries(&m_ExposedVariables);
		m_InspectedAsset->GenerateAssetEntries(&m_ExposedAssets);
		m_InspectedAsset->GenerateConnectionEntries(&m_ExposedRefs);
	}

	void AssetInspector::OnImGuiRender()
	{
		if (ImGui::Begin("Asset Inspector##", &m_Open)) {
			const std::string& assetName = m_InspectedAsset->GetName();
			ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2);
			ImGui::Text(assetName.c_str());
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("Save")) {
				auto assetManager = GameSmith::AssetManager::GetInstance();
				const std::string filePath = assetManager->GetAssetPath(m_InspectedAsset->GetID());
				fs::path filePathObj(filePath);
				assetManager->WriteResource(m_InspectedAsset, filePathObj.parent_path().string());
			}

			ImGui::Separator();

			auto& groupings = m_InspectedAsset->GetExposedGroupings();
			for (auto& group : groupings) {
				bool showVariables = true;
				if (group.first != "") {
					showVariables = ImGui::CollapsingHeader(group.first.c_str());
				}

				if (showVariables) {
					auto& variableMap = m_ExposedVariables;
					auto& refMap = m_ExposedRefs;
					auto& assetMap = m_ExposedAssets;

					for (auto& variableName : group.second) {
						if (variableMap.contains(variableName)) {
							auto parameter = variableMap[variableName];
							GenerateVariableUI(parameter);
						}
						else if (refMap.contains(variableName)) {
							auto connection = refMap[variableName];
							InputReference(variableName, connection);
						}
						else if (assetMap.contains(variableName)) {
							auto assetRef = assetMap[variableName];
							InputReference(variableName, assetRef);
						}
					}
				}
			}
		}
		ImGui::End();
	}

	void AssetInspector::OnUpdate(float dt)
	{
		if (!m_Open) {
			CloseWindow();
		}

		m_InspectedAsset->BootstrapVariableRegistry(m_ExposedVariables);
		m_InspectedAsset->BootstrapAssetRegistry(m_ExposedAssets);
		m_InspectedAsset->BootstrapConnectionRegistry(m_ExposedRefs);
	}
};