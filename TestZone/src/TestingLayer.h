#pragma once
#include "EngineDependenciesOnly.h"

class TestingLayer : public GameSmith::Layer
{
public:
	TestingLayer();
	void OnImGuiRender() override;
	void OnUpdate() override;
};

