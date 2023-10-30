#pragma once
#include "ProjectGE/Core/Core.h"
#include "ProjectGE/ResourceAssets/ShaderAsset.h"
#include <glm/gtc/type_ptr.hpp>

namespace ProjectGE {
	enum class ShaderConstantType {
		Global,
		Instance
	};


	struct GloablShaderData {
		glm::mat4 VP;
		glm::vec3 CameraWorldPos;
		float pad;
		glm::vec4 LightWorldPos;
		glm::vec3 MainLightColor;
	};

	struct ShaderSet {
		Ref<ShaderAsset> shaders[STAGE_NUM];
	};
};
