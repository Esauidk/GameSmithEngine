#pragma once
#include "EngineDependenciesOnly.h"

class SandBoxLayer : public GameSmith::Layer
{
public:
	SandBoxLayer();
	void OnImGuiRender() override;
	void OnUpdate() override;
	void EventSubscribe(const std::vector<GameSmith::EventDispatcherBase*>& dispatchers, bool overlay) override {}
private:
	std::vector<GameSmith::Connection<GameSmith::GameObject>> m_GameObjects;

	glm::vec3 pos1;
	glm::vec3 rot1;
	glm::vec3 pos2;
	glm::vec3 rot2;
	glm::vec3 pos3;
	glm::vec3 rot3;

	glm::vec3 lightPos;
	glm::vec3 lightDir;
	glm::vec3 lightColor;

	GameSmith::OrthoCamera m_Cam;
	GameSmith::PerspectiveCamera m_PerpCam;

	bool switchPerp = false;
	bool switchLight = false;
};

