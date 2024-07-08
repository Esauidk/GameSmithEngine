#pragma once
#include "gepch.h"
#include "GameSmithEngine/Core/Core.h"

#define GE_IMPL_CONNECT_CONTENTLIBRARY(ContentLibraryClass) \
extern "C" GE_EXPLCIT_EXPORT GameSmith::ContentLibrary* ConnectToContentLibrary() { \
	return new ContentLibraryClass(); \
}\

namespace GameSmith {


	class GE_API ContentLibrary {
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
	};

	struct DynamicLibraryDetails {
		std::string LibraryName = "";
		std::string LibraryPath = "";
		void* loadedLib = nullptr;
		Ref<ContentLibrary> ContentLib = nullptr;
		bool isLoaded = false;
	};

	class GE_API ContentLibraryManager
	{
	public:
		ContentLibraryManager();

		inline static ContentLibraryManager* GetInstance() { return s_Instance; }
		void DiscoveryLibaries();

		void LoadContentLibrary(std::string libraryName);
		void UnloadContentLibrary(std::string libraryName);
	private:
		std::unordered_map<std::string, DynamicLibraryDetails> m_Libraries;
		static ContentLibraryManager* s_Instance;
	};
};



