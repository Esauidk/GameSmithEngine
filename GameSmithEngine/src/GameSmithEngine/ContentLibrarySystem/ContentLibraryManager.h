#pragma once
#include "GameSmithEngine/Core/Core.h"

namespace GameSmith {
	class ContentLibrary {
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
	};

	class GE_API ContentLibraryManager
	{
	public:
		void LoadContentLibrary(std::string libraryName);
	};

	extern ContentLibrary* ConnectToContentLibrary();
};



