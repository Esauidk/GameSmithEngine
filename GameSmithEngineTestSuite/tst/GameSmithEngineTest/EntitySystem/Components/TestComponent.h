#pragma once
#include "GameSmithEngine/EntitySystem/Components/Component.h"

class TestComponent : public GameSmith::Component {
public:
	TestComponent(GameSmith::GameObject* gameObject, GameSmith::Transform* transform) : Component(gameObject, transform), m_InitRan(false), m_UpdateRan(false) {}
	virtual void OnStart() override { m_InitRan = true; }
	virtual void OnUpdate(float dt) { m_UpdateRan = true; }
	virtual void OnDestroy() override {}
	bool GetInitCheck() { return m_InitRan; }
	bool GetUpdateCheck() { return m_UpdateRan; }
	virtual int GetPriority() const { return 0; }
	virtual std::string GetName() const override { return "Test Component"; }

	virtual GameSmith::Ref<char> Serialize() override { return nullptr; }
	virtual void Serialize(char* byteStream, unsigned int availableBytes) override {}
	virtual unsigned int RequireSpace() const override { return 0; }
	virtual void Deserialize(char* inData, unsigned int size) override {}
private:
	bool m_InitRan;
	bool m_UpdateRan;
};
