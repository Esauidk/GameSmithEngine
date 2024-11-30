#include "gepch.h"
#include "ContentLibraryManager.h"
#include "GameSmithEngine/Core/Log.h"

#include <filesystem>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

namespace GameSmith {
	ContentLibraryManager* ContentLibraryManager::s_Instance = nullptr;

	typedef ContentLibrary* (*ConnectLibraryFunc)();

	ContentLibraryManager::ContentLibraryManager(){}

	void ContentLibraryManager::Init()
	{
		if (s_Instance == nullptr) {
			s_Instance = new ContentLibraryManager();
		}
	}

	void ContentLibraryManager::Shutdown()
	{
		if (s_Instance != nullptr) {
			delete s_Instance;
			s_Instance = nullptr;
		}
	}

	void ContentLibraryManager::DiscoveryLibaries()
	{
		for (const auto& dirEntry : recursive_directory_iterator("ContentLibraries")) {
			if (dirEntry.is_regular_file()) {
				std::string fileName = dirEntry.path().filename().string();
				std::string path = dirEntry.path().string();

				if (path.ends_with(".dll")) {
					DynamicLibraryDetails details;
					details.LibraryName = fileName;
					details.LibraryPath = path;
					m_Libraries.insert({ fileName, details });
				}

				std::cout << dirEntry << std::endl;
			}

		}
	}

	void ContentLibraryManager::LoadContentLibrary(std::string libraryName)
	{
		GE_CORE_ASSERT(m_Libraries.contains(libraryName), "This DLL/ContentLibrary is not registered for use");
		auto libEntry = m_Libraries.find(libraryName);
		GE_CORE_ASSERT(!libEntry->second.isLoaded, "This DLL/ContentLibrary is already loaded");

#ifdef GE_PLATFORM_WINDOWS
		auto module = LoadLibraryA(libEntry->second.LibraryPath.c_str());
		libEntry->second.loadedLib = (void*)module;

		auto func = (ConnectLibraryFunc)GetProcAddress(module, "ConnectToContentLibrary");
		libEntry->second.ContentLib = GameSmith::Ref<ContentLibrary>(func());
#endif

		libEntry->second.isLoaded = true;
		libEntry->second.ContentLib->Init();
	}

	void ContentLibraryManager::UnloadContentLibrary(std::string libraryName)
	{
		GE_CORE_ASSERT(m_Libraries.contains(libraryName), "This DLL/ContentLibrary is not registered for use");
		auto libEntry = m_Libraries.find(libraryName);
		GE_CORE_ASSERT(libEntry->second.isLoaded, "This DLL/ContentLibrary is not loaded");

		libEntry->second.ContentLib->Shutdown();
		libEntry->second.ContentLib = nullptr;

#ifdef GE_PLATFORM_WINDOWS
		FreeLibrary((HMODULE)libEntry->second.loadedLib);
#endif

		libEntry->second.isLoaded = false;
	}

	void ContentLibraryManager::LoadAllLibraries()
	{
		for (auto& entry : m_Libraries) {
			LoadContentLibrary(entry.first);
		}
	}
};
