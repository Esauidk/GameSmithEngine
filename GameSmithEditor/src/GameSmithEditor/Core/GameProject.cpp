#include "GameProject.h"
#include <filesystem>
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"
#include "GameSmithEditor/Core/EditorMenu.h"
#include "GameSmithEditor/Utils/SystemCallUtils.h"

namespace fs = std::filesystem;

namespace GameSmithEditor {
	//REGISTER_EDITOR_MENU_OPTION(Files_CreateProject, &MenuCreateProject);
	REGISTER_EDITOR_MENU_OPTION(Files_LoadProject, &MenuLoadProject);

	GameSmith::Scope<GameProject> GameProject::s_CurProject = nullptr;

	void MenuCreateProject() {
		std::string rootFolder;
		if (PickFolderDialog(std::filesystem::current_path().string(), &rootFolder)) {
			GameSmithEditor::GameProject::CreateProject("TestProjectDir2", rootFolder);
		}
	}

	void MenuLoadProject() {
		std::string rootFolder;
		if (PickFolderDialog(std::filesystem::current_path().string(), &rootFolder)) {
			GameSmithEditor::GameProject::LoadProject(rootFolder);
		}
		
	}

	static void SetupProjectResources() {
		auto resourceManager = GameSmith::AssetManager::GetInstance();
		resourceManager->SetAssetDirectory(GameProject::GetAssetFolder());
		resourceManager->ScanResources();

		auto contentLibraryManager = GameSmith::ContentLibraryManager::GetInstance();
		contentLibraryManager->SetLibrarySearchPath(GameProject::GetCacheFolder());
		contentLibraryManager->ScanForContentLibraries();
	}

	void GameProject::CreateProject(std::string projectName, std::string projectPath)
	{
		if (s_CurProject == nullptr) {
			s_CurProject = GameSmith::Scope<GameProject>(new GameProject());
		}

		std::string fullProjectPath = std::format("{0}\\{1}", projectPath, projectName);
		fs::create_directories(fullProjectPath);

		// Copy preset files
		std::string curDir = fs::current_path().string();
		std::string parentDir = fs::current_path().parent_path().string();
		fs::copy(std::format(
			"{0}\\{1}",
			curDir,
			PROJECT_PRESET_FOLDER),
			fullProjectPath,
			fs::copy_options::recursive | fs::copy_options::overwrite_existing
		);

		// Symlink Engine Source
		if (fs::exists(std::format("{0}\\{1}", fullProjectPath, "GameSmithEngine"))) {
			fs::remove(std::format("{0}\\{1}", fullProjectPath, "GameSmithEngine"));
		}

		// TODO: Update this with installation bundled with editor
		fs::create_directory_symlink(std::format(
			"{0}\\{1}",
			parentDir,
			"GameSmithEngine"),
			std::format(
				"{0}\\{1}",
				fullProjectPath,
				"GameSmithEngine"
			)
		);


		const std::string folders[] = { PROJECT_ASSET_FOLDER_NAME, PROJECT_CACHE_FOLDER_NAME, PROJECT_SRC_FOLDER_NAME, PROJECT_EDITOR_RESOURCE_FOLDER_NAME };
		const unsigned int folderCount = 4;

		for (unsigned int i = 0; i < folderCount; i++) {
			std::string fullPath = std::format("{0}\\{1}", fullProjectPath, folders[i]);
			fs::create_directories(fullPath);
		}

		s_CurProject->SetRootFolder(fullProjectPath);
		s_CurProject->SetProjectName(projectName);

		auto buf = s_CurProject->Serialize();

		unsigned int serializedSize = s_CurProject->RequiredSpace();
		GameSmith::BinaryStreamWriter writer(buf.get(), serializedSize);
		writer.MoveCurPtr(serializedSize);

		writer.CommitToFile(std::format("{0}\\{1}.{2}", fullProjectPath, projectName, s_CurProject->GetFileExtension()));

		SetupProjectResources();
	}

	void GameProject::LoadProject(std::string projectFolder)
	{
		std::string projectName = projectFolder.substr(projectFolder.find_last_of("\\"));
		auto reader = GameSmith::BinaryStreamReader::ReadDirectlyFromFile(std::format("{0}\\{1}.GameSmithPrj", projectFolder, projectName));

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

