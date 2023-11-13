#pragma once
#include "GameSmithEngine/Rendering/RenderAgnostics/LightingSystem/LightSource.h"

namespace GameSmith {
	class PointLight : public LightSource {
	public:
		PointLight() : m_LightPos(0, 0, 0, 1), m_LightColor(1, 1, 1) {}
		inline void SetLightPosition(glm::vec3& dir) { m_LightPos = glm::vec4(dir, 1); }
		inline void SetLightColor(glm::vec3& color) { m_LightColor = color; }
		virtual glm::vec4& GetLightVector() { return m_LightPos; };
		virtual glm::vec3& GetLightColor() { return m_LightColor; };
	private:
		glm::vec3 m_LightColor;
		glm::vec4 m_LightPos;
	};
};