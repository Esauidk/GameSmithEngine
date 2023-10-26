#pragma once
#include <glm/gtc/matrix_transform.hpp>

namespace ProjectGE {
	class LightSource {
	public:
		virtual glm::vec3& GetLightVector() = 0;
		virtual glm::vec3& GetLightColor() = 0;
	};
};
