#pragma once

#include "EngineDependenciesOnly.h"

class TestComponentLayer : public GameSmith::Layer
{
public:
	TestComponentLayer();
	void OnAttach() override;
	void OnImGuiRender() override;
	void OnUpdate() override;
	void EventSubscribe(const std::vector<GameSmith::EventDispatcherBase*>& dispatchers, bool overlay) override {}
private:
	GameSmith::GameObject gameObject;
};

