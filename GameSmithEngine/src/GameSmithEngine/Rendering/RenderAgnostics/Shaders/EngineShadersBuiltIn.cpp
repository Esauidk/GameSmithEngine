#include <gepch.h>
#include "EngineShadersBuiltIn.h"
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderPresetsFactory.h"

namespace GameSmith {
	GE_REGISTER_PRESET_SHADER(GameSmithShaderCore, R"(
#include "Camera.hlsl"
#include "Light.hlsl"
#include "Instance.hlsl"
	)")
};