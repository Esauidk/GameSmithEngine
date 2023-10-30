#pragma once
#include "ProjectGE/Rendering/RenderAgnostics/LightingSystem/LightSource.h"

namespace ProjectGE {
	class PointLight : public LightSource {
	public:
		inline void SetLightPosition(glm::vec3& dir) { m_LightPos = glm::vec4(dir, 1); }
		inline void SetLightColor(glm::vec3& color) { m_LightColor = color; }
		virtual glm::vec4& GetLightVector() { return m_LightPos; };
		virtual glm::vec3& GetLightColor() { return m_LightColor; };
	private:
		glm::vec3 m_LightColor;
		glm::vec4 m_LightPos;
	};
};