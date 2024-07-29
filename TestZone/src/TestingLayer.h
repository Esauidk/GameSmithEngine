#pragma once
#include "GameSmithEngine.h"

class TestingLayer : public GameSmith::Layer
{
public:
	TestingLayer();
	void OnImGuiRender() override;
	void OnUpdate() override;
private:
	GameSmith::Ref<GameSmith::RenderTexture> m_RenderTex;
};

