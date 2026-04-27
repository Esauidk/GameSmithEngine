#pragma once
#include "GameSmithEngine.h"
#include "EditorWindow.h"

#define PROJECT_NAME_SIZE 256
#define PROJECT_PATH_SIZE 256
namespace GameSmithEditor {
	class ProjectCreatorWindow : public EditorWindow
	{
	public:
		ProjectCreatorWindow() : EditorWindow("Project Creator") {}
		void OnImGuiRender() override;
		void OnUpdate(float dt) override;
	private:
		bool m_Open = true;
		bool m_Complete = false;
		bool m_PopupOpen = false;
		std::array<char, PROJECT_NAME_SIZE> m_ProjectName = { '\0' };
		std::array<char, PROJECT_PATH_SIZE> m_ProjectPath = { '\0' };
	};
};
