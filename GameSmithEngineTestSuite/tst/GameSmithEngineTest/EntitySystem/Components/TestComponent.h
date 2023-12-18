#pragma once
#include "GameSmithEngine/EntitySystem/Components/Component.h"

class TestComponent : public GameSmith::Component {
public:
	TestComponent(GameSmith::GameObject* gameObject, GameSmith::Transform* transform) : Component(gameObject, transform), m_InitRan(false), m_UpdateRan(false) {}
	virtual void OnStart() override { m_InitRan = true; }
	virtual void OnUpdate() { m_UpdateRan = true; }
	virtual void OnDestroy() override {}
	bool GetInitCheck() { return m_InitRan; }
	bool GetUpdateCheck() { return m_UpdateRan; }
	virtual int GetPriority() const { return 0; }
private:
	bool m_InitRan;
	bool m_UpdateRan;
};
