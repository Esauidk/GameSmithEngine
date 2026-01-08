#pragma once
#include "gepch.h"
#include "GameSmithEngine/ContentLibrarySystem/ContentLibrary.h"
#include "GameSmithEngine/Core/Core.h"

namespace GameSmith {
	typedef void* LibraryHandle;

	struct DynamicLibraryDetails {
		std::string LibraryName = "";
		std::string LibraryPath = "";
		LibraryHandle loadedLib = nullptr;
		Ref<ContentLibrary> ContentLib = nullptr;
	};

	class GE_API ContentLibraryManager
	{
	public:
		static void Init(const std::unordered_map<std::string, std::string>& startingLibraries);
		static void Shutdown();
		~ContentLibraryManager();

		static ContentLibraryManager* GetInstance();

		void LoadContentLibrary(std::string libraryName, std::string libraryPath);
		void UnloadContentLibrary(std::string libraryName);

	private:
		ContentLibraryManager();
	private:
		std::unordered_map<std::string, DynamicLibraryDetails> m_Libraries;
		static ContentLibraryManager* s_Instance;
	};
};



