#include "SimulationContentView.h"
#include "imgui.h"

namespace GameSmithEditor {
	void SmulationContentView::OnImGuiRender()
	{
		ImGui::Begin("Simulation Content View");
		if (ImGui::Button("Create GameObject")) {
			auto gm = GameSmith::GameObjectManager::GetInstance();
			auto object = gm->CreateGameObject();
			m_NamesStd.push_back(object.lock()->GetName());
			m_Objects.push_back(m_NamesStd[m_NamesStd.size() - 1].c_str());
		}

		ImGui::BeginListBox("Objects", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()));
		for (int i = 0; i < m_NamesStd.size(); i++) {
			bool selected = (m_Selection == i);
			if (ImGui::Selectable(m_NamesStd[i].c_str(), selected)) {
				m_Selection = i;
			}

			if (selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
		ImGui::End();
	}

	void SmulationContentView::OnUpdate()
	{
	}
};

