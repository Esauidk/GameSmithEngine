#pragma once
#include <glm/gtc/type_ptr.hpp>

namespace ProjectGE {
	enum Stages {
		STAGE_VERTEX,
		STAGE_PIXEL,
		STAGE_NUM
	};

	enum class ShaderConstantType {
		Global,
		Instance
	};

	struct GloablShaderData {
		glm::mat4 VP;
	};
};
