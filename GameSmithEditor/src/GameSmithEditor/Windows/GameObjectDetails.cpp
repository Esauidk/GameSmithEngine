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
				std::string name = comp.lock()->GetName();
				if (ImGui::CollapsingHeader(name.c_str())) {
					//TODO: Reflect the components on the gameobject
					auto variableMap = m_ExposedVariables.find(name)->second;
					for (auto& entry : variableMap) {
						auto parameter = entry.second;
						if (parameter->GetType() == GameSmith::ContainerDataType::Float) {
							std::string parameterName = parameter->GetName();
							ImGui::InputFloat(parameterName.c_str(), (float*)parameter->GetCharData());
						}
					}
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

				if (ImGui::Button("Add", ImVec2(120, 0))) { 
					auto newComponent = m_Object.lock()->AddComponent<GameSmith::TestComponent>();
					m_Components.push_back(newComponent);
					std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> compMap;
					newComponent.lock()->GenerateVariableEntries(&compMap);
					m_ExposedVariables.insert({ newComponent.lock()->GetName(), compMap });

					ImGui::CloseCurrentPopup(); 
				}
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
			m_ExposedVariables.clear();

			if (!object.expired()) {
				object.lock()->GetComponents<GameSmith::Component>(&m_Components);
				m_Object = object;

				for (auto comp : m_Components) {
					std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> compMap;
					comp.lock()->GenerateVariableEntries(&compMap);
					m_ExposedVariables.insert({ comp.lock()->GetName(), compMap });
				}
			}
		}
		else {
			for (auto comp : m_Components) {
				std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>>& compMap = m_ExposedVariables.find(comp.lock()->GetName())->second;
				comp.lock()->BootstrapRegistry(compMap);
			}
		}
	}

};

