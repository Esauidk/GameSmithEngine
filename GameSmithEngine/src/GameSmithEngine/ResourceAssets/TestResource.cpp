#include "gepch.h"
#include "TestResource.h"
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	void TestResource::Init()
	{
		GE_CORE_INFO("Loaded resource!");
	}

	void TestResource::Destroy()
	{
		GE_CORE_INFO("Destroyed Resource!");
	}
};

