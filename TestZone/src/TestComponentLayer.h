#pragma once

#include "EngineDependenciesOnly.h"

class TestComponentLayer : public ProjectGE::Layer
{
public:
	TestComponentLayer();
	void OnAttach() override;
	void OnImGuiRender() override;
	void OnUpdate() override;
	void EventSubscribe(const std::vector<ProjectGE::EventDispatcherBase*>& dispatchers, bool overlay) override {}
private:
	ProjectGE::GameObject gameObject;
};

