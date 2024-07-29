#pragma once
#include "gepch.h"
#include "GameSmithEngine.h"

namespace GameSmithEditor {
	void MenuCreateProject();
	void MenuLoadProject();

	class GameProject
	{
	public:
		static void CreateProject(std::string projectName, std::string projectPath);
		static void LoadProject(std::string projectFolder);

		static std::string GetRootFolder() { return s_CurProject->m_RootFolder; }
		static std::string GetProjectName() { return s_CurProject->m_ProjectName; }
		static bool IsLoaded() { return s_CurProject != nullptr; }

	private:
		GameProject() = default;
		inline void SetRootFolder(std::string root) { m_RootFolder = root; };
		inline void SetProjectName(std::string name) { m_ProjectName = name; }
	private:
		static const std::string s_Folders[];
		static const unsigned int s_FolderCount;
		static GameSmith::Ref<GameProject> s_CurProject;
	private:
		std::string m_RootFolder;
		std::string m_ProjectName;
	};
};


