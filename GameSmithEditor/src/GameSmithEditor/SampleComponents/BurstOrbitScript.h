#pragma once
#include "GameSmithEditor/EngineDependenciesOnly.h"
#include <random>

namespace GameSmithEditor {
	class BurstOrbitScript : public GameSmith::Component {
	public:
		BurstOrbitScript(GameSmith::GameObject* gameObject, GameSmith::Transform* transform) : Component(gameObject, transform) {
			m_Registry.AddExposedVariable("Num of Objects", { &m_NumObjs, GameSmith::ContainerDataType::Int });
			m_Registry.AddExposedVariable("Origin Point", { glm::value_ptr(m_Center), GameSmith::ContainerDataType::Float3 });
			m_Registry.AddExposedVariable("Time Scale", { &m_TimeScale, GameSmith::ContainerDataType::Float });
		}

		virtual void OnStart() override { }
		virtual void OnUpdate(float dt);
		virtual void OnDestroy() override {}
		virtual std::string GetName() const override { return "Burst Demo Script"; };
		virtual int GetPriority() const { return 0; }
	private:
		std::vector<GameSmith::Connection<GameSmith::GameObject>> m_Objs;
		glm::vec3 m_Center = {0, 0, 0};
		float m_TimeScale = 1;
		int m_NumObjs = 0;

		// Rando paraneters
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f, 2 * glm::pi<float>() };
		std::uniform_real_distribution<float> ddist{ 0.0f, 2 * glm::pi<float>() };
		std::uniform_real_distribution<float> odist{ 0.0f, 2 * glm::pi<float>() };
		std::uniform_real_distribution<float> rdist{ 6.0f, 20.0f };
	};
};