#pragma once
#include "EngineDependenciesOnly.h"
#include <random>

class DemoLayer : public GameSmith::Layer
{
public:
	DemoLayer();
	void OnImGuiRender() override;
	void OnUpdate() override;
private:
	// Demo Resources
	GameSmith::PerspectiveCamera m_PerpCam;
	GameSmith::Ref<GameSmith::RenderTexture> m_RenderTex;
	std::vector<GameSmith::Connection<GameSmith::GameObject>> m_GameObjects;
	GameSmith::Connection<GameSmith::GameObject> m_LightObject;
	GameSmith::Connection<GameSmith::MeshRenderer> m_LightMesh;
	GameSmith::Ref<GameSmith::MaterialAsset> m_LightMatAsset;
	GameSmith::Ref<GameSmith::MaterialAsset> m_ColorMatAsset;
	GameSmith::Ref<GameSmith::MaterialAsset> m_LummieThiefAsset;

	glm::vec3 m_LightPos;
	glm::vec3 m_LightDir;
	glm::vec3 m_LightColor;
	int m_ShaderItem = 0;
	int m_MeshItem = 0;
	float m_TimeScale = 1;

	char m_MeshLocation[500] = "";

	// Flags
	bool m_CreateGameObject = false;
	bool m_SwitchLight = false;
	bool m_ClearGameObjects = false;

	// Rando paraneters
	std::mt19937 rng{ std::random_device{}() };
	std::uniform_real_distribution<float> adist{0.0f, 2 * glm::pi<float>()};
	std::uniform_real_distribution<float> ddist{ 0.0f, 2 * glm::pi<float>()};
	std::uniform_real_distribution<float> odist{ 0.0f, 2 * glm::pi<float>()};
	std::uniform_real_distribution<float> rdist{ 6.0f, 20.0f };
};


class DemoScript : public GameSmith::Component {
public:
	DemoScript(GameSmith::GameObject* gameObject, GameSmith::Transform* transform) : Component(gameObject, transform) {
		m_Transform->SetCoordinateFrame(&m_t);
	}
	virtual void OnStart() override { }
	virtual void OnUpdate(float dt);
	virtual void OnDestroy() override {}
	virtual std::string GetName() const override { return "Demo Script"; };
	virtual int GetPriority() const { return 0; }
	inline void SetTimeScale(float scale) { m_TimeScale = scale; }
	inline void SetOribitSpeeds(glm::vec3 speeds) { m_OribitSpeeds = speeds; }
	inline void SetLocalRotationSpeed(glm::vec3 speeds) { m_LocalSpeeds = speeds; }
	inline void SetOrigin(glm::vec3 newOrigin) { m_t.SetPosition(newOrigin); }
	inline void SetSphereSize(float newRadius) { m_Transform->SetPosition(glm::vec3(newRadius, 0, 0)); }
private:
	GameSmith::Transform m_t;
	glm::vec3 m_OribitSpeeds;
	glm::vec3 m_LocalSpeeds;
	float m_TimeScale = 1;
};
