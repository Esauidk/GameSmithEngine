#include <gepch.h>
#include "LightShaderData.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderPresetsFactory.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"

namespace GameSmith {
	static_assert(static_cast<int>(ShaderConstantType::Light) == 1);

	GE_REGISTER_PRESET_SHADER(Light, R"(
cbuffer LightData : register(b1)
{
	float4 LIGHT_POSITION;
	float3 LIGHT_COLOR;
};
	)")
};