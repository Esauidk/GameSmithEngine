#include "gepch.h"
#include "TestComponent.h"
#include "ProjectGE/Core/Log.h"

namespace ProjectGE {
	void TestComponent::OnStart()
	{
		GE_CORE_INFO("Started Test Component");
	}

	void TestComponent::OnUpdate()
	{
		GE_CORE_INFO("Updated Test Component");
	}

	void TestComponent::OnDestroy()
	{
		GE_CORE_INFO("Destroyed Test Component");
	}
};

