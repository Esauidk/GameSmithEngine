#pragma once
#include "Component.h"

namespace ProjectGE {
	class TestComponent : public Component {
	public:
		TestComponent(GameObject* gameObject, Transform* transform) : Component(gameObject, transform) {}
		virtual void OnStart() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;
	};
};
