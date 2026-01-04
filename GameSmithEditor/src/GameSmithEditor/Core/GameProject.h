#pragma once
#include "gepch.h"
#include "GameSmithEngine.h"

#define PROJECT_ASSET_FOLDER_NAME "Assets"
#define PROJECT_EDITOR_RESOURCE_FOLDER_NAME "EditorResources"
#define PROJECT_CACHE_FOLDER_NAME "Cache"
#define PROJECT_SRC_FOLDER_NAME "Src"

namespace GameSmithEditor {
	void MenuCreateProject();
	void MenuLoadProject();

	class GameProject : public GameSmith::Serializeable
	{
	public:
		static void CreateProject(std::string projectName, std::string projectPath);
		static void LoadProject(std::string projectFolder);

		static std::string GetRootFolder() { return s_CurProject->m_RootFolder; }
		static std::string GetAssetFolder() { return std::format("{0}\\{1}", s_CurProject->m_RootFolder, PROJECT_ASSET_FOLDER_NAME); }
		static std::string GetEditorResourceFolder() { return std::format("{0}\\{1}", s_CurProject->m_RootFolder, PROJECT_EDITOR_RESOURCE_FOLDER_NAME); }
		static std::string GetProjectName() { return s_CurProject->m_ProjectName; }
		static bool IsLoaded() { return s_CurProject != nullptr; }


		virtual GameSmith::Ref<char> Serialize() override;
		virtual void Serialize(char* byteStream, unsigned int availableBytes) override;
		virtual unsigned int RequiredSpace() const override;
		virtual void Deserialize(char* inData, unsigned int size) override;

		SERIAL_FILE(GameSmith Project, GameSmithPrj)

	private:
		GameProject() = default;
		inline void SetRootFolder(std::string root) { m_RootFolder = root; };
		inline void SetProjectName(std::string name) { m_ProjectName = name; }
	private:
		static GameSmith::Scope<GameProject> s_CurProject;
	private:
		std::string m_RootFolder;
		std::string m_ProjectName;
	};
};


