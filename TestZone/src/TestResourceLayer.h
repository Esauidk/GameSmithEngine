#pragma once
#include "EngineDependenciesOnly.h"

class TestResourceLayer : public ProjectGE::Layer
{
public:
	TestResourceLayer(UINT threshold);
	void OnImGuiRender() override;
	void OnUpdate() override;
	void EventSubscribe(const std::vector<ProjectGE::EventDispatcherBase*>& dispatchers, bool overlay) override {}
private:
	ProjectGE::Ref<ProjectGE::TestResource> m_Resource;
	UINT m_Iteration = 0;
	UINT m_Threshold = 10;
	bool m_Pressed = false;
};

