#include "GameObjectDetails.h"
#include "SimulationContentView.h"
#include "imgui.h"

namespace GameSmithEditor {
	static void GenerateVariableUI(GameSmith::Ref<GameSmith::ParameterContainer> container) {
		auto name = container->GetName();

		switch (container->GetType()) {
		case GameSmith::ContainerDataType::Float:
			ImGui::InputFloat(name.c_str(), (float*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Float2:
			ImGui::InputFloat2(name.c_str(), (float*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Float3:
			ImGui::InputFloat3(name.c_str(), (float*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Float4:
			ImGui::InputFloat4(name.c_str(), (float*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Int:
			ImGui::InputInt(name.c_str(), (int*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Int2:
			ImGui::InputInt2(name.c_str(), (int*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Int3:
			ImGui::InputInt3(name.c_str(), (int*)container->GetCharData());
			break;
		case GameSmith::ContainerDataType::Int4:
			ImGui::InputInt4(name.c_str(), (int*)container->GetCharData());
			break;
		}
	}

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
						GenerateVariableUI(parameter);
					}
				}
			}

			if (ImGui::Button("AddComponent")) {
				ImGui::OpenPopup("Component Picker");
			}

			if (ImGui::BeginPopupModal("Component Picker")) {
				if (ImGui::BeginListBox("Component Options", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() * 0.5f))) {
					auto compRegistry = GameSmith::ComponentRegistry::GetInstance();
					std::vector<std::string> availableComps;
					compRegistry->ListRegisteredComponents(&availableComps);

					for (auto entry : availableComps) {
						if (ImGui::Selectable(entry.c_str(), entry == m_CurCompSelection)) {
							m_CurCompSelection = entry;
						}
					}

					
					ImGui::EndListBox();
				}

				if (ImGui::Button("Add", ImVec2(120, 0))) { 
					auto newComponent = m_Object.lock()->AddComponent(m_CurCompSelection);
					m_Components.push_back(newComponent);
					std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> compMap;
					newComponent.lock()->GenerateVariableEntries(&compMap);
					m_ExposedVariables.insert({ newComponent.lock()->GetName(), compMap });

					m_CurCompSelection = "";
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

