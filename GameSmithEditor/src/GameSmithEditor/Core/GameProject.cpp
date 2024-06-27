#include "GameProject.h"
#include <filesystem>
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

namespace GameSmithEditor {
	const std::string GameProject::s_Folders[] = {"Assets", "Cache", "Src"};
	const unsigned int GameProject::s_FolderCount = 3;

	GameSmith::Ref<GameProject> GameProject::s_CurProject = nullptr;

	static void SetupProjectResources() {
		auto resourceManager = GameSmith::ResourceManager::GetInstance();
		resourceManager->SetAssestDirectory(GameProject::GetRootFolder());
		resourceManager->ScanResources();
	}

	void GameProject::CreateProject(std::string projectName, std::string projectPath)
	{
		if (s_CurProject == nullptr) {
			s_CurProject = GameSmith::Ref<GameProject>(new GameProject());
		}

		for (unsigned int i = 0; i < s_FolderCount; i++) {
			std::string fullPath = std::format("{0}\\{1}\\{2}", projectPath, projectName, s_Folders[i]);
			std::filesystem::create_directories(fullPath);
		}

		std::string projRoot = std::format("{0}\\{1}", projectPath, projectName);

		s_CurProject->SetRootFolder(projRoot);
		s_CurProject->SetProjectName(projectName);

		unsigned int size = projectName.size() + 1;
		GameSmith::ResourceAssetWriter writer(size);
		writer.WriteString(projectName);
		writer.CommitToFile(std::format("{0}\\{1}\\{2}.GameSmithPrj", projectPath, projectName, projectName));

		SetupProjectResources();
	}

	void GameProject::LoadProject(std::string projectFolder)
	{
		auto reader = GameSmith::ResourceAssetReader::ReadDirectlyFromFile(std::format("{0}\\{1}.GameSmithPrj", projectFolder, "TestProjectDir"));

		std::string prjName = reader.GetString();

		if (s_CurProject == nullptr) {
			s_CurProject = GameSmith::Ref<GameProject>(new GameProject());
		}

		s_CurProject->SetRootFolder(projectFolder);
		s_CurProject->SetProjectName(prjName);

		SetupProjectResources();
	}
};

