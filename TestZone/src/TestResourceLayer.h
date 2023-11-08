#pragma once
#include "EngineDependenciesOnly.h"

class TestResourceLayer : public GameSmith::Layer
{
public:
	TestResourceLayer();
	void OnImGuiRender() override;
	void OnUpdate() override;
	void EventSubscribe(const std::vector<GameSmith::EventDispatcherBase*>& dispatchers, bool overlay) override {}
private:
	GameSmith::Ref<GameSmith::MeshAsset> m_Resource;
	GameSmith::Ref<GameSmith::TextureAsset> m_TexResource;
	bool m_Pressed = false;
};

