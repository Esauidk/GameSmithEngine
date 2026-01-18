#pragma once
#include "gepch.h"
#include "GameSmithEngine/ContentLibrarySystem/ContentLibrary.h"
#include "GameSmithEngine/Core/Core.h"

namespace GameSmith {
	typedef void* LibraryHandle;

	struct DynamicLibraryDetails {
		LibraryHandle loadedLib = nullptr;
		Ref<ContentLibrary> ContentLib = nullptr;
	};

	class GE_API ContentLibraryManager
	{
	public:
		static void Init();
		static void Shutdown();
		~ContentLibraryManager();

		static ContentLibraryManager* GetInstance();

		/// <summary>
		/// This sets the search path for content libraries to be loaded from.
		/// This should be treated as an override functionality.
		/// 
		/// Default path/searching functionality will be determined by the engine configuration
		/// 
		/// </summary>
		/// <param name="path"></param>
		inline void SetLibrarySearchPath(std::string path) { m_LibrarySearchPath = path; FlushLoadedLibraries(); }
		void ScanForContentLibraries();

		void FlushLoadedLibraries();
		void LoadAllContentLibraries();

		void LoadContentLibrary(std::string libraryName);
		void UnloadContentLibrary(std::string libraryName);

	private:
		ContentLibraryManager();
	private:
		std::string m_LibrarySearchPath;
		std::unordered_map<std::string, DynamicLibraryDetails> m_LoadedLibraries;
		std::unordered_map<std::string, std::string> m_LibraryPaths;

	private:
		static ContentLibraryManager* s_Instance;
	};
};



