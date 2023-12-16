#pragma once
#include "Component.h"

namespace GameSmith {
	class TestComponent : public Component {
	public:
		TestComponent(GameObject* gameObject, Transform* transform) : Component(gameObject, transform) {}
		virtual void OnStart() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;
		virtual int GetPriority() override { return 0; };
	};
};
