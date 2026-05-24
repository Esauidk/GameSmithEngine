#include <gepch.h>
#include "CameraShaderData.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderPresetsFactory.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"

namespace GameSmith {
	static_assert(static_cast<int>(ShaderConstantType::Camera) == 0);

	GE_REGISTER_PRESET_SHADER(Camera, R"(
cbuffer CameraBuffer : register(b0)
{
	matrix VP;
	float3 CAMERA_WORLD_POS;
}
	)")
};