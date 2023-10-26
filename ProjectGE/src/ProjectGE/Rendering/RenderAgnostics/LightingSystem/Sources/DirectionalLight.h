#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/LightingSystem/LightSource.h"

namespace ProjectGE {
	class DirectionalLight : public LightSource
	{
	public:
		inline void SetLightVector(glm::vec3& dir) { m_LightDirection = dir; }
		inline void SetLightColor(glm::vec3& color) { m_LightColor = color; }
		inline virtual glm::vec3& GetLightVector() override { return m_LightDirection; };
		inline virtual glm::vec3& GetLightColor() override { return m_LightColor; };
	private:
		glm::vec3 m_LightColor;
		glm::vec3 m_LightDirection;
	};
};


