#include "gepch.h"
#include "ConfigManager.h"

#include "GameSmithEngine/ResourceManagement/ResourceLoaders/HeapResourceLoader.h"

#include <filesystem>

namespace fs = std::filesystem;
using recursive_directory_iterator = fs::recursive_directory_iterator;

namespace GameSmith {

	ConfigManager* ConfigManager::s_Instance = nullptr;

	void ConfigManager::Init()
	{
		if (s_Instance == nullptr) {
			s_Instance = new ConfigManager();
		}
	}

	void ConfigManager::Shutdown()
	{
		if (s_Instance != nullptr) {
			delete s_Instance;
		}
	}

	ConfigManager::ConfigManager() : m_FileLoader(new HeapResourceLoader())
	{
		// This current iteration of the config manager will look for all config files in a predefined folder in the current working directory
		std::string configFolderPath = fs::current_path().string() + "/" + CONFIG_FOLDER;
		if (fs::exists(configFolderPath)) {
			for (const auto& dirEntry : recursive_directory_iterator(configFolderPath)) {
				if (dirEntry.is_regular_file()) {
					std::string fileName = dirEntry.path().filename().string();
					std::string path = dirEntry.path().string();
					
					char* fileData = nullptr;

					// We currently have a predefined set of config files we look for, so we can just do simple string comparisons here
					if (fileName == std::string(ASSET_MANAGEMENT_CONFIG_FILENAME)) {
						unsigned int size;
						fileData = m_FileLoader->LoadResource(path, &size);

						m_AssetManagementConfig.Deserialize(fileData, size);
					}
					else if (fileName == std::string(CONTENT_LIBRARY_CONFIG_FILENAME)) {
						unsigned int size;
						fileData = m_FileLoader->LoadResource(path, &size);

						m_ContentLibraryConfig.Deserialize(fileData, size);
					}
					else if (fileName == std::string(RENDERING_CONFIG_FILENAME)) {
						unsigned int size;
						fileData = m_FileLoader->LoadResource(path, &size);

						m_RenderingConfig.Deserialize(fileData, size);
					}


					if (fileData != nullptr) {
						m_FileLoader->CleanResource(fileData);
					}
				}
			}
		}
	}
};

