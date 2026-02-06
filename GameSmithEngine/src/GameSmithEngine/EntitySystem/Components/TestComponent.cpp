#include "gepch.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/EntitySystem/Components/ComponentFactory.h"
#include "GameSmithEngine/EntitySystem/GameObject.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/GameChunkAsset.h"
#include "TestComponent.h"

namespace GameSmith {
	GE_REGISTERCOMPONENT(TestComponent)

	TestComponent::TestComponent(GameObject* gameObject, Transform* transform) : Component(gameObject, transform) {
		m_Registry.AddExposedVariable("Test Variable1", (void*)&expose1, ContainerDataType::Float);
		m_Registry.AddExposedVariable("Test Variable2", (void*)glm::value_ptr(expose2), ContainerDataType::Float2);
		m_Registry.AddExposedVariable("Test Variable3", (void*)glm::value_ptr(expose3), ContainerDataType::Float3);
		m_Registry.AddExposedVariable("Test Variable4", (void*)glm::value_ptr(expose4), ContainerDataType::Float4);
		m_Registry.AddExposedVariable("Test Variable5", (void*)&expose5, ContainerDataType::Int);
		m_Registry.AddExposedVariable("Test Variable6", (void*)&expose6, ContainerDataType::Int2);
		m_Registry.AddExposedVariable("Test Variable7", (void*)&expose7, ContainerDataType::Int3);
		m_Registry.AddExposedVariable("Test Variable8", (void*)&expose8, ContainerDataType::Int4);
		m_Registry.AddExposedConnection<GameObject>("TestRef", (Connection<GameSmith::IDObjectInterface>*) & m_TestRef, CLASS_TO_STRING(GameObject));
		m_Registry.AddExposedConnection<TestComponent>("TestRef1", (Connection<GameSmith::IDObjectInterface>*) & m_TestCompRef, CLASS_TO_STRING(TestComponent));
		m_Registry.AddExposedAsset<GameChunkAsset>("TestRef2", (Ref<GameSmith::IAsset>*) & m_GameChunkAsset, CLASS_TO_STRING(GameChunkAsset));
	}

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

		if (!m_TestRef.expired()) {
			GE_CORE_INFO("GameObject Name: {0}", m_TestRef.lock()->GetName());
		}
	}

	void TestComponent::OnDestroy()
	{
		GE_CORE_INFO("Destroyed Test Component");
	}
};

