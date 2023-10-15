#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Shader.h"
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

	struct ShaderSet {
		Ref<Shader> shaders[STAGE_NUM];
	};
};
