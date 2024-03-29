#pragma once
#include "GameSmithEngine/Rendering/RenderAgnostics/LightingSystem/LightSource.h"

namespace GameSmith {
	class DirectionalLight : public LightSource
	{
	public:
		DirectionalLight() : m_LightColor(1, 1, 1), m_LightDirection(0, 0, 0, 0) {}
		inline void SetLightDirection(glm::vec3& dir) { m_LightDirection = glm::vec4(dir, 0); }
		inline void SetLightColor(glm::vec3& color) { m_LightColor = color; }
		inline virtual glm::vec4& GetLightVector() override { return m_LightDirection; };
		inline virtual glm::vec3& GetLightColor() override { return m_LightColor; };
	private:
		glm::vec3 m_LightColor;
		glm::vec4 m_LightDirection;
	};
};


