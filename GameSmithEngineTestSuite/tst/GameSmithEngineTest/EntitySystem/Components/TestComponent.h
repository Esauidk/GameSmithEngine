#pragma once
#include "GameSmithEngine/EntitySystem/Components/Component.h"

class TestComponent : public GameSmith::Component {
public:
	TestComponent(GameSmith::GameObject* gameObject, GameSmith::Transform* transform) : Component(gameObject, transform), m_UpdateRan(false) {}
	virtual void OnStart() override {}
	virtual void OnUpdate() { m_UpdateRan = true; }
	virtual void OnDestroy() override {}
	bool GetUpdateCheck() { return m_UpdateRan; }
private:
	bool m_UpdateRan;
};
