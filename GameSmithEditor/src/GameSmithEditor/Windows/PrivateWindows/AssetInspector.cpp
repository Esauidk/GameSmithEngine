#include "AssetInspector.h"
#include "imgui.h"

namespace GameSmithEditor {
	void AssetInspector::OnImGuiRender()
	{
		if (ImGui::Begin("Asset Inspector##", &m_Open)) {
			const std::string& assetName = m_InspectedAsset->GetName();
			ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2);
			ImGui::Text(assetName.c_str());
			ImGui::PopItemWidth();
			ImGui::Separator();
		}
		ImGui::End();
	}

	void AssetInspector::OnUpdate(float dt)
	{
		if (!m_Open) {
			CloseWindow();
		}
	}
};