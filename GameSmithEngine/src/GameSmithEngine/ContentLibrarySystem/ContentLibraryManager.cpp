#include "gepch.h"
#include "ContentLibraryManager.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/EngineConfiguration/ConfigManager.h"

#define CONNECT_LIBRARY_FUNC_NAME "ConnectToContentLibrary"
#define LIBRARY_FILE_EXTENSION ".dll"

namespace fs = std::filesystem;
using recursive_directory_iterator = fs::recursive_directory_iterator;

namespace GameSmith {
	ContentLibraryManager* ContentLibraryManager::s_Instance = nullptr;

	typedef ContentLibrary* (*ConnectLibraryFunc)();

	ContentLibraryManager::ContentLibraryManager(){}

	void ContentLibraryManager::Init()
	{
		if (s_Instance == nullptr) {
			s_Instance = new ContentLibraryManager();

			auto contentLibConfig = ConfigManager::GetInstance()->GetContentLibraryConfig();
			s_Instance->SetLibrarySearchPath(contentLibConfig.GetContentLibraryDirectory());
		}
	}

	void ContentLibraryManager::Shutdown()
	{
		if (s_Instance != nullptr) {
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	ContentLibraryManager::~ContentLibraryManager()
	{
		std::vector<std::string> keys;
		for (auto& libEntry : m_LoadedLibraries) {
			keys.push_back(libEntry.first);
		}

		for (auto& key : keys) {
			UnloadContentLibrary(key);
		}
	}

	ContentLibraryManager* ContentLibraryManager::GetInstance()
	{
		return s_Instance;
	}

	void ContentLibraryManager::ScanForContentLibraries()
	{
		if (m_LibrarySearchPath != "") {
			m_LibraryPaths.clear();

			// Current search method is directly looking for the files in a path. Future improvements is baking paths as a file during build time
			for (const auto& dirEntry : recursive_directory_iterator(m_LibrarySearchPath)) {
				if (dirEntry.is_regular_file()) {
					std::string fileNameWithExt = dirEntry.path().filename().string();
					std::string path = dirEntry.path().string();

					if (fileNameWithExt.ends_with(LIBRARY_FILE_EXTENSION)) {
						std::string fileName = fileNameWithExt.substr(0, fileNameWithExt.find_last_of('.'));
						m_LibraryPaths[fileName] = path;
					}
				}
			}
		}
	}

	void ContentLibraryManager::FlushLoadedLibraries()
	{
		std::vector<std::string> loadedLibs;
		for (auto& entry : m_LoadedLibraries) {
			loadedLibs.push_back(entry.first);
		}

		for (auto& libName : loadedLibs) {
			UnloadContentLibrary(libName);
		}
	}

	void ContentLibraryManager::LoadAllContentLibraries()
	{
		std::vector<std::string> knownLibs;
		for (auto& entry : m_LibraryPaths) {
			knownLibs.push_back(entry.first);
		}

		for (auto& libName : knownLibs) {
			LoadContentLibrary(libName);
		}
	}

	void ContentLibraryManager::LoadContentLibrary(std::string libraryName)
	{
		GE_CORE_ASSERT(!m_LoadedLibraries.contains(libraryName), "This DLL/ContentLibrary is already loaded");
		GE_CORE_ASSERT(m_LibraryPaths.contains(libraryName), "This DLL/ContentLibrary is not recognized");

#ifdef GE_PLATFORM_WINDOWS
		DynamicLibraryDetails libDetails;

		auto libraryPath = m_LibraryPaths[libraryName];
		auto module = LoadLibraryA(libraryPath.c_str());
		libDetails.loadedLib = (LibraryHandle)module;

		if (module == nullptr) {
			GE_CORE_ERROR("Failed to load library: {0}", libraryPath);
			return;
		}

		auto func = (ConnectLibraryFunc)GetProcAddress(module, CONNECT_LIBRARY_FUNC_NAME);
		if (func == nullptr) {
			GE_CORE_ERROR("Failed to find function {0} in library: {1}", CONNECT_LIBRARY_FUNC_NAME, libraryName);
			FreeLibrary(module);
			return;
		}

		libDetails.ContentLib = GameSmith::Ref<ContentLibrary>(func());
#endif
		libDetails.ContentLib->Init();

		m_LoadedLibraries[libraryName] = libDetails;
	}

	void ContentLibraryManager::UnloadContentLibrary(std::string libraryName)
	{
		GE_CORE_ASSERT(m_LoadedLibraries.contains(libraryName), "This DLL/ContentLibrary is currently in use");
		auto libEntry = m_LoadedLibraries.find(libraryName);

		libEntry->second.ContentLib->Shutdown();
		libEntry->second.ContentLib = nullptr;

#ifdef GE_PLATFORM_WINDOWS
		FreeLibrary((HMODULE)libEntry->second.loadedLib);
#endif
		m_LoadedLibraries.erase(libEntry);
	}
};
