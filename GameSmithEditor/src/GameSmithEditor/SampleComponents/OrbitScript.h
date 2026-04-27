#pragma once
#include "GameSmithEngine.h"

namespace GameSmithEditor {
    class OrbitScript : public GameSmith::Component {
    public:
        OrbitScript(GameSmith::GameObject* gameObject, GameSmith::Transform* transform) : Component(gameObject, transform) {
            GetTransform()->SetCoordinateFrame(&m_t);
            m_Registry.AddExposedVariable("Origin Point", GameSmith::ContainerDataType::Float3, glm::value_ptr(m_Origin));
            m_Registry.AddExposedVariable("Oribit Radius", GameSmith::ContainerDataType::Float , &m_Radius);
            m_Registry.AddExposedVariable("Oribit Speed", GameSmith::ContainerDataType::Float3, glm::value_ptr(m_OribitSpeeds));
            m_Registry.AddExposedVariable("Local Rotation Speed", GameSmith::ContainerDataType::Float3, glm::value_ptr(m_LocalSpeeds));
            m_Registry.AddExposedVariable("Time Scale", GameSmith::ContainerDataType::Float, &m_TimeScale);
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
