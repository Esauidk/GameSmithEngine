#pragma once
#include "EngineDependenciesOnly.h"

class TestResourceLayer : public ProjectGE::Layer
{
public:
	TestResourceLayer();
	void OnImGuiRender() override;
	void OnUpdate() override;
	void EventSubscribe(const std::vector<ProjectGE::EventDispatcherBase*>& dispatchers, bool overlay) override {}
private:
	ProjectGE::Ref<ProjectGE::TestResource> m_Resource;
	ProjectGE::Ref<ProjectGE::TextureAsset> m_TexResource;
	bool m_Pressed = false;
};

