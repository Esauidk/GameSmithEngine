#pragma once
#include "gepch.h"
#include "GameSmithEngine/Core/Core.h"

#define GE_IMPL_CONNECT_CONTENTLIBRARY(ContentLibraryClass) \
extern "C" GE_EXPLCIT_EXPORT GameSmith::ContentLibrary* ConnectToContentLibrary() { \
	return new ContentLibraryClass(); \
}\

namespace GameSmith {

	typedef void* LibraryHandle;

	class GE_API ContentLibrary {
	public:
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
	};

	struct DynamicLibraryDetails {
		std::string LibraryName = "";
		std::string LibraryPath = "";
		LibraryHandle loadedLib = nullptr;
		Ref<ContentLibrary> ContentLib = nullptr;
	};

	class GE_API ContentLibraryManager
	{
	public:
		static void Init();
		static void Shutdown();

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



