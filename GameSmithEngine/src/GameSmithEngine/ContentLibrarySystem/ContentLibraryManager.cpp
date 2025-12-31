#include "gepch.h"
#include "ContentLibraryManager.h"
#include "GameSmithEngine/Core/Log.h"

#define CONNECT_LIBRARY_FUNC_NAME "ConnectToContentLibrary"

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

	ContentLibraryManager* ContentLibraryManager::GetInstance()
	{
		return s_Instance;
	}

	void ContentLibraryManager::LoadContentLibrary(std::string libraryName, std::string libraryPath)
	{
		GE_CORE_ASSERT(!m_Libraries.contains(libraryName), "This DLL/ContentLibrary is already loaded");

#ifdef GE_PLATFORM_WINDOWS
		DynamicLibraryDetails libDetails;
		libDetails.LibraryName = libraryName;
		libDetails.LibraryPath = libraryPath;

		auto module = LoadLibraryA(libDetails.LibraryPath.c_str());
		libDetails.loadedLib = (LibraryHandle)module;

		if (module == nullptr) {
			GE_CORE_ERROR("Failed to load library: {0}", libDetails.LibraryPath);
			return;
		}

		auto func = (ConnectLibraryFunc)GetProcAddress(module, CONNECT_LIBRARY_FUNC_NAME);
		if (func == nullptr) {
			GE_CORE_ERROR("Failed to find function {0} in library: {1}", CONNECT_LIBRARY_FUNC_NAME, libDetails.LibraryPath);
			FreeLibrary(module);
			return;
		}

		libDetails.ContentLib = GameSmith::Ref<ContentLibrary>(func());
#endif
		libDetails.ContentLib->Init();

		m_Libraries[libraryName] = libDetails;
	}

	void ContentLibraryManager::UnloadContentLibrary(std::string libraryName)
	{
		GE_CORE_ASSERT(m_Libraries.contains(libraryName), "This DLL/ContentLibrary is currently in use");
		auto libEntry = m_Libraries.find(libraryName);

		libEntry->second.ContentLib->Shutdown();
		libEntry->second.ContentLib = nullptr;

#ifdef GE_PLATFORM_WINDOWS
		FreeLibrary((HMODULE)libEntry->second.loadedLib);
#endif
	}
};
