#pragma once
#include "GameSmithEngine/Core/Core.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/ShaderTypes/ShaderAsset.h"
#include <glm/gtc/type_ptr.hpp>

namespace GameSmith {
	enum class ShaderConstantType {
		Camera = 0,
		Light = 1,
		Material = 2,
		Instance = 3
	};


	struct GloablShaderData {
		glm::mat4 VP;
		glm::vec3 CameraWorldPos;
		float pad;
		glm::vec4 LightWorldPos;
		glm::vec3 MainLightColor;
	};

	struct ShaderSet {
		Ref<Shader> shaders[STAGE_NUM];

		std::size_t operator()(const ShaderSet& key) const {
			std::size_t hash = 0;
			// Combine the hash codes of the struct members

			for (unsigned int i = 0; i < STAGE_NUM; i++) {
				Stages stage = (Stages)i;

				if (shaders[i] != nullptr) {
					hash ^= shaders[i]->GetHash();
				}
			}
			
			return hash;
		}
	};
};
