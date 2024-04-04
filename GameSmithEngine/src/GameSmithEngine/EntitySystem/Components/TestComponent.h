#pragma once
#include "Component.h"

namespace GameSmith {
	class TestComponent : public Component {
	public:
		TestComponent(GameObject* gameObject, Transform* transform) : Component(gameObject, transform), testExposedVariable(0) {
			m_Registry.AddExposedVariable("Test Variable", { &testExposedVariable, ContainerDataType::Float });
		}
		virtual void OnStart() override;
		virtual void OnUpdate(float dt) override;
		virtual void OnDestroy() override;
		virtual int GetPriority() const override { return 0; };
		virtual std::string GetName() const override { return "TestComponent"; };
	private:
		float testExposedVariable;
	};
};
