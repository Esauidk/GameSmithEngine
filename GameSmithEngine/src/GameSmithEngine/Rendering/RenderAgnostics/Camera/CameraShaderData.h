#pragma once
#include <glm/gtc/type_ptr.hpp>

namespace GameSmith {
	// Pad to 80 bytes (HLSL float3's have a 16-byte aligned) so CPU layout matches HLSL cbuffer std140 rules
	struct CameraShaderData {
		glm::mat4 VP;
		glm::vec3 CameraWorldPos;
		float pad;

		bool operator==(const CameraShaderData& other) const {
			return VP == other.VP && CameraWorldPos == other.CameraWorldPos;
		}
	};
};