#include "GameObjectDetails.h"
#include "SimulationContentView.h"
#include "GameSmithEditor/CustomWidgets/ReferenceInputWidget.h"
#include "imgui.h"

namespace GameSmithEditor {

	REGISTER_WINDOW_DEFAULT_CALLBACK(Windows_GameObjectDetails, GameObjectDetails);


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
		//InputReference();
		if (!m_Object.expired()) {
			auto name = m_Object.lock()->GetName();
			memcpy(m_InputName, name.c_str(), name.size());
			ImGui::PushItemWidth(ImGui::GetWindowWidth() / 2);
			ImGui::SameLine();
			ImGui::InputText("##GameObjectName", m_InputName, GAMEOBJECT_NAME_SIZE);
			if (ImGui::IsItemDeactivatedAfterEdit()) {
				m_Object.lock()->SetName(std::string(m_InputName, GAMEOBJECT_NAME_SIZE));
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			
			
			ImGuiStyle& style = ImGui::GetStyle();
			auto defaultStyle = style.Colors[ImGuiCol_Button];
			style.Colors[ImGuiCol_Button] = ImVec4(0.95, 0.125, 0.344, 1);
			if (ImGui::Button("Delete")) {
				GameSmith::GameObjectManager::GetInstance()->DestroyGameObject(m_Object);
			}
			style.Colors[ImGuiCol_Button] = defaultStyle;

			ImGui::Separator();
			if (ImGui::CollapsingHeader("Transform")) {
				auto lockTransform = m_Object.lock()->GetTransform().lock();
				glm::vec3 pos = lockTransform->GetPosition();
				glm::vec3 rot = lockTransform->GetRotation();
				glm::vec3 scale = lockTransform->GetScale();

				ImGui::InputFloat3("Position", glm::value_ptr(pos));
				ImGui::InputFloat3("Rotation", glm::value_ptr(rot));
				ImGui::InputFloat3("Scale", glm::value_ptr(scale));

				lockTransform->SetPosition(pos);
				lockTransform->SetRotation(rot);
				lockTransform->SetScale(scale);
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
					auto newComponent = m_Object.lock()->AddComponent(m_CurCompSelection).lock();
					m_Components.push_back(newComponent);
					std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> compMap;
					newComponent->GenerateVariableEntries(&compMap);
					m_ExposedVariables.insert({ newComponent->GetName(), compMap });

					// TODO: Temporary
					if (m_CurCompSelection == "MeshRenderer") {
						auto renderer = GameSmith::CastPtr<GameSmith::MeshRenderer>(newComponent);
						renderer->SetMesh(GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MeshAsset>(GameSmith::ID(250309786, 50043, 18339, 17854270087279912363)));

						auto mat = GameSmith::ResourceManager::GetInstance()->GetResource<GameSmith::MaterialAsset>(GameSmith::ID(2652798047, 63418, 17489, 6150598651790595990));
						for (unsigned int i = 0; i < renderer->GetMaterialSlots(); i++) {
							renderer->SetMaterial(i, mat);
						}


					}

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

