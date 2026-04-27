#include "Inspector.h"
#include "GameSmithEditor/CustomWidgets/ReferenceInputWidget.h"
#include "imgui.h"
#include "GameSmithEditor/Utils/ExposedVariableWidgetConverter.h"

namespace GameSmithEditor {

	GameSmith::Connection<GameSmith::GameObject> Inspector::s_SelectedObject = GameSmith::Ref<GameSmith::GameObject>();

	REGISTER_WINDOW_DEFAULT_CALLBACK(Windows_Inspector, Inspector);

	void Inspector::OnImGuiRender()
	{
		if (ImGui::Begin("Inspector", &m_Open)) {
			if (!m_Object.expired()) {
				auto name = m_Object.lock()->GetName();
				strcpy(m_InputName, name.c_str());
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
				style.Colors[ImGuiCol_Button] = ImVec4(0.95f, 0.125f, 0.344f, 1.0f);
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
						auto& groupings = comp.lock()->GetExposedGroupings();
						for (auto& group : groupings) {
							bool showVariables = true;
							if (group.first != "") {
								showVariables = ImGui::CollapsingHeader(group.first.c_str());
							}

							if (showVariables) {
								auto& variableMap = m_ExposedVariables[name];
								auto& refMap = m_ExposedRefs[name];
								auto& assetMap = m_ExposedAssets[name];

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
						//TODO: Reflect the components on the gameobject
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
						std::unordered_map<std::string, GameSmith::Ref<GameSmith::ConnectionContainer>> refMap;
						std::unordered_map<std::string, GameSmith::Ref<GameSmith::AssetRefContainer>> assetMap;
						newComponent->GenerateVariableEntries(&compMap);
						newComponent->GenerateConnectionEntries(&refMap);
						newComponent->GenerateAssetEntries(&assetMap);
						m_ExposedVariables.insert({ newComponent->GetName(), compMap });
						m_ExposedRefs.insert({ newComponent->GetName(), refMap });
						m_ExposedAssets.insert({ newComponent->GetName(), assetMap });

						m_CurCompSelection = "";
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine(ImGui::GetWindowWidth() - ImGui::GetCursorPosX() - 120);
					if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
					ImGui::EndPopup();
				}
			}
		}
		
		ImGui::End();
	}

	void Inspector::OnUpdate(float dt)
	{
		if (!s_SelectedObject.expired() && (m_Object.expired() || m_Object.lock().get() != s_SelectedObject.lock().get())) {
			m_Components.clear();
			m_ExposedVariables.clear();
			m_ExposedRefs.clear();
			m_ExposedAssets.clear();

			if (s_SelectedObject.lock().get()) {
				m_Object = s_SelectedObject;
				m_Object.lock()->GetComponents<GameSmith::Component>(&m_Components);

				for (auto comp : m_Components) {
					std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>> compMap;
					std::unordered_map<std::string, GameSmith::Ref<GameSmith::ConnectionContainer>> refMap;
					std::unordered_map<std::string, GameSmith::Ref<GameSmith::AssetRefContainer>> assetMap;
					comp.lock()->GenerateVariableEntries(&compMap);
					comp.lock()->GenerateConnectionEntries(&refMap);
					comp.lock()->GenerateAssetEntries(&assetMap);
					m_ExposedVariables.insert({ comp.lock()->GetName(), compMap });
					m_ExposedRefs.insert({ comp.lock()->GetName(), refMap });
					m_ExposedAssets.insert({ comp.lock()->GetName(), assetMap });
				}
			}
		}
		else {
			for (auto comp : m_Components) {
				std::unordered_map<std::string, GameSmith::Ref<GameSmith::ParameterContainer>>& compMap = m_ExposedVariables.find(comp.lock()->GetName())->second;
				std::unordered_map<std::string, GameSmith::Ref<GameSmith::ConnectionContainer>>& refMap = m_ExposedRefs.find(comp.lock()->GetName())->second;
				std::unordered_map<std::string, GameSmith::Ref<GameSmith::AssetRefContainer>>& assetMap = m_ExposedAssets.find(comp.lock()->GetName())->second;
				comp.lock()->BootstrapVariableRegistry(compMap);
				comp.lock()->BootstrapConnectionRegistry(refMap);
				comp.lock()->BootstrapAssetRegistry(assetMap);
			}
		}

		if (!m_Open) {
			CloseWindow();
		}
	}

	void Inspector::SetInspectedGameObject(GameSmith::Connection<GameSmith::GameObject> object)
	{
		s_SelectedObject = object;
	}
};

