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
		glm::vec3 LightWorldPos;
		float pad2;
		glm::vec3 MainLightDir;
		float pad3;
		glm::vec3 MainLightColor;
	};

	struct ShaderSet {
		Ref<ShaderAsset> shaders[STAGE_NUM];
	};
};
