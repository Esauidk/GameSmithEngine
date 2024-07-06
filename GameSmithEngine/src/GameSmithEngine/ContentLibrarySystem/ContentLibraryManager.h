#pragma once
namespace GameSmith {
	class ContentLibrary {
		virtual void Init() = 0;
		virtual void Shutdown() = 0;
	};

	class ContentLibraryManager
	{
	public:
		void LoadContentLibrary(std::string libraryName);
	};

	extern ContentLibrary* ConnectToContentLibrary();
};



