#include "GameSmithEditor/Core/GameProject.h"
#include "GameSmithEditor/Utils/SystemCallUtils.h"
#include "imgui.h"
#include "ProjectCreatorWindow.h"

namespace GameSmithEditor {
	REGISTER_WINDOW_DEFAULT_CALLBACK(Files_CreateProject, ProjectCreatorWindow);

	void ProjectCreatorWindow::OnImGuiRender()
	{
		ImGuiWindowFlags windowFlags = 0;
		windowFlags |= ImGuiWindowFlags_NoBackground;
		windowFlags |= ImGuiWindowFlags_NoTitleBar;

		if (ImGui::Begin("Project Create Window", &m_Open, windowFlags)) {
			if (!m_PopupOpen) {
				ImGui::OpenPopup("Project Creator");
				m_PopupOpen = true;
			}

			if (ImGui::BeginPopupModal("Project Creator")) {
				ImGui::InputText("Project Name", m_ProjectName.data(), m_ProjectName.size());
				ImGui::InputText("Project Path", m_ProjectPath.data(), m_ProjectPath.size());
				ImGui::SameLine();
				if (ImGui::Button("Browse", ImVec2(120, 0))) {
					std::string path;
					if (PickFolderDialog("", &path)) {
						path.copy(m_ProjectPath.data(), m_ProjectPath.size());
					}
				};

				ImGui::Separator();

				const float footer_height = ImGui::GetFrameHeightWithSpacing();
				ImGui::SetCursorPosY(ImGui::GetWindowHeight() - footer_height);
				if (ImGui::Button("Create", ImVec2(120, 0))) {
					m_Complete = true;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}

		ImGui::End();

	}

	void ProjectCreatorWindow::OnUpdate(float dt)
	{
		if (m_Complete) {
			GameProject::CreateProject(
				std::string(m_ProjectName.data(), strlen(m_ProjectName.data())),
				std::string(m_ProjectPath.data(), strlen(m_ProjectPath.data()))
			);
			m_Open = false;
		}

		if (!m_Open) {
			CloseWindow();
		}
	}
};