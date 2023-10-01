#include "gepch.h"
#include "TestResource.h"
#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	void TestResource::Init()
	{
		GE_CORE_INFO("Loaded resource!");
	}

	void TestResource::Destroy()
	{
		GE_CORE_INFO("Destroyed Resource!");
	}
};

