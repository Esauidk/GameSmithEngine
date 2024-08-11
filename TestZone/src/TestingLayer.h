#pragma once
#include "GameSmithEngine.h"

class TestLayerComponent : public GameSmith::Component {
public:
	TestLayerComponent(GameSmith::GameObject* gameObject, GameSmith::Transform* transform) : Component(gameObject, transform) {
		m_Registry.AddExposedRef<GameSmith::GameObject>("TestRef", (GameSmith::Connection<GameSmith::IDObject>*) & m_TestRef);
	}

	virtual void OnStart() override {};
	virtual void OnUpdate(float dt) override {};
	virtual void OnDestroy() override {};
	virtual int GetPriority() const override { return 0; };
	virtual std::string GetName() const override { return "TestLayerComponent"; };
private:
	GameSmith::Connection<GameSmith::GameObject> m_TestRef;
};

class TestingLayer : public GameSmith::Layer
{
public:
	TestingLayer();
	void OnImGuiRender() override;
	void OnUpdate() override;
private:
	GameSmith::Ref<GameSmith::RenderTexture> m_RenderTex;
};

