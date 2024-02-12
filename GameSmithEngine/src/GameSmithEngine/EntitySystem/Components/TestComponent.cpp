#include "gepch.h"
#include "TestComponent.h"
#include "GameSmithEngine/Core/Log.h"

namespace GameSmith {
	void TestComponent::OnStart()
	{
		GE_CORE_INFO("Started Test Component");
	}

	void TestComponent::OnUpdate(float dt)
	{
		GE_CORE_INFO("Updated Test Component");
	}

	void TestComponent::OnDestroy()
	{
		GE_CORE_INFO("Destroyed Test Component");
	}
};

