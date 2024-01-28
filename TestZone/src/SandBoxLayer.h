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
	GameSmith::Connection<GameSmith::Transform> m_t1;
	GameSmith::Connection<GameSmith::Transform> m_t2;
	GameSmith::Connection<GameSmith::Transform> m_t3;

	GameSmith::Ref<GameSmith::GameChunk> m_Chunk2;
	GameSmith::Ref<GameSmith::RenderTexture> m_Tex;
	GameSmith::Ref<GameSmith::TextureAsset> m_OutsideAsset;
	GameSmith::Ref<GameSmith::Texture2D> m_OutsideTex;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GameView;

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
	bool applyPartition2 = false;
	bool doPartialRendering = false;

	float renderX;
	float renderY;
	float renderWidth;
	float renderHeight;
};

