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

			for (auto& entry : m_ExposedVariables) {
				auto parameter = entry.second;
				GenerateVariableUI(parameter);
			}

			for (auto& entry : m_ExposedRefs) {
				InputReference(entry.first, entry.second);
			}

			for (auto& entry : m_ExposedAssets) {
				InputReference(entry.first, entry.second);
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