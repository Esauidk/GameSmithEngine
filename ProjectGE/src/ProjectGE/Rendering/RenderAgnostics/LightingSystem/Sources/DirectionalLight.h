#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/LightingSystem/LightSource.h"

namespace ProjectGE {
	class DirectionalLight : public LightSource
	{
	public:
		inline void SetLightDirection(glm::vec3& dir) { m_LightDirection = glm::vec4(dir, 0); }
		inline void SetLightColor(glm::vec3& color) { m_LightColor = color; }
		inline virtual glm::vec4& GetLightVector() override { return m_LightDirection; };
		inline virtual glm::vec3& GetLightColor() override { return m_LightColor; };
	private:
		glm::vec3 m_LightColor;
		glm::vec4 m_LightDirection;
	};
};


