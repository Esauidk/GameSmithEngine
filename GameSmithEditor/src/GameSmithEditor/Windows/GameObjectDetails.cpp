#include "GameObjectDetails.h"
#include "SimulationContentView.h"
#include "imgui.h"

namespace GameSmithEditor {
	void GameObjectDetails::OnImGuiRender()
	{
		ImGui::Begin("GameObject Details");
		if (!m_Object.expired()) {
			if (!m_Object.expired() && ImGui::CollapsingHeader("Transform")) {

			}

			for (auto comp : m_Components) {
				if (ImGui::CollapsingHeader("Component")) {
					//TODO: Reflect the components on the gameobject
					ImGui::Text("Hi");
				}
			}

			if (ImGui::Button("AddComponent")) {
				ImGui::OpenPopup("Component Picker");
			}

			if (ImGui::BeginPopupModal("Component Picker")) {
				if (ImGui::BeginListBox("Component Options", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() * 0.5))) {
					if (ImGui::Selectable("Test Component", false)) {
						
					}
					ImGui::EndListBox();
				}

				if (ImGui::Button("Add", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
				ImGui::SameLine(ImGui::GetWindowWidth()-ImGui::GetCursorPosX() - 120);
				if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}
		}
		
		ImGui::End();
	}

	void GameObjectDetails::OnUpdate()
	{
		auto object = SimulationContentView::GetCurrentObject();
		if (m_Object.expired() || m_Object.lock().get() != object.lock().get()) {
			m_Components.clear();

			if (!object.expired()) {
				object.lock()->GetComponents<GameSmith::Component>(&m_Components);
				m_Object = object;
			}
			
		}
	}
};

