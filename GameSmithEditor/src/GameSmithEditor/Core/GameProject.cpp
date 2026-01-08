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
		auto resourceManager = GameSmith::AssetManager::GetInstance();
		resourceManager->SetAssetDirectory(GameProject::GetRootFolder());
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

		auto buf = s_CurProject->Serialize();

		GameSmith::BinaryStreamWriter writer(buf.get(), s_CurProject->RequiredSpace());
		writer.CommitToFile(std::format("{0}\\{1}\\{2}.{3}", projectPath, projectName, projectName, s_CurProject->GetFileExtension()));

		SetupProjectResources();
	}

	void GameProject::LoadProject(std::string projectFolder)
	{
		auto reader = GameSmith::BinaryStreamReader::ReadDirectlyFromFile(std::format("{0}\\{1}.GameSmithPrj", projectFolder, "TestProjectDir2"));

		if (s_CurProject == nullptr) {
			s_CurProject = GameSmith::Scope<GameProject>(new GameProject());
		}

		s_CurProject->SetRootFolder(projectFolder);
		s_CurProject->Deserialize(reader.GetCurPtr(), reader.GetRemainingBytes());

		SetupProjectResources();
	}

	GameSmith::Ref<char> GameProject::Serialize()
	{
		unsigned size = RequiredSpace();
		auto buf = GameSmith::Ref<char>(new char[size]);

		Serialize(buf.get(), size);

		return buf;
	}

	void GameProject::Serialize(char* byteStream, unsigned int availableBytes)
	{
		unsigned int size = RequiredSpace();
		GE_CORE_ASSERT(availableBytes >= size, "Not enough bytes to serialize GameProject structure");
		GameSmith::BinaryStreamWriter writer(byteStream, availableBytes);
		writer.WriteString(m_ProjectName);
	}

	unsigned int GameProject::RequiredSpace() const
	{
		return (unsigned int)m_ProjectName.size() + 1;
	}

	void GameProject::Deserialize(char* inData, unsigned int size)
	{
		GameSmith::BinaryStreamReader reader(inData, size);
		SetProjectName(reader.GetString());
	}
};

