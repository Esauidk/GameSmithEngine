#include "GameProject.h"
#include <filesystem>
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"
#include "GameSmithEditor/Core/EditorMenu.h"
#include "GameSmithEditor/Utils/SystemCallUtils.h"

namespace GameSmithEditor {
	REGISTER_EDITOR_MENU_OPTION(Files_CreateProject, &MenuCreateProject);
	REGISTER_EDITOR_MENU_OPTION(Files_LoadProject, &MenuLoadProject);

	GameSmith::Scope<GameProject> GameProject::s_CurProject = nullptr;

	void MenuCreateProject() {
		std::string rootFolder = PickFolderDialog("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone");
		GameSmithEditor::GameProject::CreateProject("TestProjectDir2", rootFolder);
	}

	void MenuLoadProject() {
		std::string rootFolder = PickFolderDialog("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone");
		GameSmithEditor::GameProject::LoadProject(rootFolder);
	}

	static void SetupProjectResources() {
		auto resourceManager = GameSmith::ResourceManager::GetInstance();
		resourceManager->SetAssestDirectory(GameProject::GetRootFolder());
		resourceManager->ScanResources();
	}

	void GameProject::CreateProject(std::string projectName, std::string projectPath)
	{
		if (s_CurProject == nullptr) {
			s_CurProject = GameSmith::Scope<GameProject>(new GameProject());
		}

		const std::string folders[] = { PROJECT_ASSET_FOLDER_NAME, PROJECT_CACHE_FOLDER_NAME, PROJECT_SRC_FOLDER_NAME, PROJECT_EDITOR_RESOURCE_FOLDER_NAME };
		const unsigned int folderCount = 4;

		for (unsigned int i = 0; i < folderCount; i++) {
			std::string fullPath = std::format("{0}\\{1}\\{2}", projectPath, projectName, folders[i]);
			std::filesystem::create_directories(fullPath);
		}

		std::string projRoot = std::format("{0}\\{1}", projectPath, projectName);

		s_CurProject->SetRootFolder(projRoot);
		s_CurProject->SetProjectName(projectName);

		unsigned int size = (unsigned int)projectName.size() + 1;
		GameSmith::ResourceAssetWriter writer(size);
		writer.WriteString(projectName);
		writer.CommitToFile(std::format("{0}\\{1}\\{2}.GameSmithPrj", projectPath, projectName, projectName));

		SetupProjectResources();
	}

	void GameProject::LoadProject(std::string projectFolder)
	{
		auto reader = GameSmith::ResourceAssetReader::ReadDirectlyFromFile(std::format("{0}\\{1}.GameSmithPrj", projectFolder, "TestProjectDir2"));

		std::string prjName = reader.GetString();

		if (s_CurProject == nullptr) {
			s_CurProject = GameSmith::Scope<GameProject>(new GameProject());
		}

		s_CurProject->SetRootFolder(projectFolder);
		s_CurProject->SetProjectName(prjName);

		SetupProjectResources();
	}
};

