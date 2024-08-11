#pragma once
#include "GameSmithEngine.h"

namespace GameSmithEditor {
	class OrbitScript : public GameSmith::Component {
	public:
		OrbitScript(GameSmith::GameObject* gameObject, GameSmith::Transform* transform) : Component(gameObject, transform) {
			m_Transform->SetCoordinateFrame(&m_t);
			m_Registry.AddExposedVariable("Origin Point", glm::value_ptr(m_Origin), GameSmith::ContainerDataType::Float3);
			m_Registry.AddExposedVariable("Oribit Radius", &m_Radius, GameSmith::ContainerDataType::Float);
			m_Registry.AddExposedVariable("Oribit Speed", glm::value_ptr(m_OribitSpeeds), GameSmith::ContainerDataType::Float3);
			m_Registry.AddExposedVariable("Local Rotation Speed", glm::value_ptr(m_LocalSpeeds), GameSmith::ContainerDataType::Float3);
			m_Registry.AddExposedVariable("Time Scale", &m_TimeScale, GameSmith::ContainerDataType::Float);
		}

		virtual void OnStart() override { }
		virtual void OnUpdate(float dt);
		virtual void OnDestroy() override {}
		virtual std::string GetName() const override { return "Demo Script"; };
		virtual int GetPriority() const override { return 0; }
		inline void SetTimeScale(float scale) { m_TimeScale = scale; }
		inline void SetOribitSpeeds(glm::vec3 speeds) { m_OribitSpeeds = speeds; }
		inline void SetLocalRotationSpeed(glm::vec3 speeds) { m_LocalSpeeds = speeds; }
		inline void SetOrigin(glm::vec3 newOrigin) { m_Origin = newOrigin; }
		inline void SetSphereSize(float newRadius) { m_Radius = newRadius; }

	private:
		GameSmith::Transform m_t;
		glm::vec3 m_OribitSpeeds = {0,0,0};
		glm::vec3 m_LocalSpeeds = {0,0,0};
		glm::vec3 m_Origin = {0,0,0};
		float m_Radius = 0;
		float m_TimeScale = 1;
	};
};
