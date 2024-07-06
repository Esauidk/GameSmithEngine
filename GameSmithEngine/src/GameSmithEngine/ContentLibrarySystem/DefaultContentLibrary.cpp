#include "gepch.h"
#include "DefaultContentLibrary.h"
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	void DefaultContentLibrary::Init()
	{
		GE_APP_INFO("Initialized Content Library");
	}

	void DefaultContentLibrary::Shutdown()
	{
		GE_APP_INFO("Shutdown Content Library");
	}
};
