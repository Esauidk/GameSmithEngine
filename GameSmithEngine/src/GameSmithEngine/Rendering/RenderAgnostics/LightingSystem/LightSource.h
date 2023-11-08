#pragma once
#include <glm/gtc/matrix_transform.hpp>

namespace GameSmith {
	class LightSource {
	public:
		virtual glm::vec4& GetLightVector() = 0;
		virtual glm::vec3& GetLightColor() = 0;
	};
};
