#pragma once
#include <glm/gtc/type_ptr.hpp>

namespace GameSmith {
	// Pad to 32 bytes (HLSL float3(4)'s have a 16-byte aligned) so CPU layout matches HLSL cbuffer std140 rules
	struct LightShaderData {
		glm::vec4 LightWorldPos;
		glm::vec3 MainLightColor;
		float pad;

		bool operator==(const LightShaderData& other) const {
			return LightWorldPos == other.LightWorldPos && MainLightColor == other.MainLightColor;
		}
	};
};