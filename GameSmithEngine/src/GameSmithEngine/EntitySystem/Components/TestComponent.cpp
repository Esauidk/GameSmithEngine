#include "gepch.h"
#include "TestComponent.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/EntitySystem/Components/ComponentFactory.h"

namespace GameSmith {
	GE_REGISTERCOMPONENT(TestComponent)

	void TestComponent::OnStart()
	{
		GE_CORE_INFO("Started Test Component");
	}

	void TestComponent::OnUpdate(float dt)
	{
		GE_CORE_INFO("{0}", expose1);
		GE_CORE_INFO("{0}, {1}", expose2.x, expose2.y);
		GE_CORE_INFO("{0}, {1}, {2}", expose3.x, expose3.y, expose3.z);
		GE_CORE_INFO("{0}, {1}, {2}, {3}", expose4.x, expose4.y, expose4.z, expose4.w);

		GE_CORE_INFO("{0}", expose5);
		GE_CORE_INFO("{0}, {1}", expose6[0], expose6[1]);
		GE_CORE_INFO("{0}, {1}, {2}", expose7[0], expose7[1], expose7[2]);
		GE_CORE_INFO("{0}, {1}, {2}, {3}", expose8[0], expose8[1], expose8[2], expose8[3]);
	}

	void TestComponent::OnDestroy()
	{
		GE_CORE_INFO("Destroyed Test Component");
	}
};

