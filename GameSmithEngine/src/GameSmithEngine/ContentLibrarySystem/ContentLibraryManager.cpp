#include "gepch.h"
#include "ContentLibraryManager.h"

namespace GameSmith {
	void ContentLibraryManager::LoadContentLibrary(std::string libraryName)
	{
		auto module = LoadLibraryA("C:\\Users\\esaus\\Desktop\\TestProjectDir\\bin\\Debug-windows-x86_64\\GameProject\\GameProject.dll");
		//GetProcAddress()
	}
};
