#pragma once
#include "Component.h"

namespace GameSmith {
	class TestComponent : public Component {
	public:
		TestComponent(GameObject* gameObject, Transform* transform) : Component(gameObject, transform) {
			m_Registry.AddExposedVariable("Test Variable1", { (void*)&expose1, ContainerDataType::Float });
			m_Registry.AddExposedVariable("Test Variable2", { (void*)glm::value_ptr(expose2), ContainerDataType::Float2 });
			m_Registry.AddExposedVariable("Test Variable3", { (void*)glm::value_ptr(expose3), ContainerDataType::Float3 });
			m_Registry.AddExposedVariable("Test Variable4", { (void*)glm::value_ptr(expose4), ContainerDataType::Float4 });
			m_Registry.AddExposedVariable("Test Variable5", { (void*)&expose5, ContainerDataType::Int });
			m_Registry.AddExposedVariable("Test Variable6", { (void*)&expose6, ContainerDataType::Int2 });
			m_Registry.AddExposedVariable("Test Variable7", { (void*)&expose7, ContainerDataType::Int3 });
			m_Registry.AddExposedVariable("Test Variable8", { (void*)&expose8, ContainerDataType::Int4 });

		}
		virtual void OnStart() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnDestroy() override;
		virtual int GetPriority() const override { return 0; };
		virtual std::string GetName() const override { return "TestComponent"; };
	private:
		float expose1 = 0;
		glm::vec2 expose2 = {};
		glm::vec3 expose3 = {};
		glm::vec4 expose4 = {};

		int expose5 = 0;
		int expose6[2] = {0};
		int expose7[3] = {0};
		int expose8[4] = {0};
	};
};
