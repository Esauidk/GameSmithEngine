#pragma once

#include "ProjectGE/Rendering/RenderAgnostics/Shaders/ShaderUtil.h"
#include <wrl.h>
#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;
namespace ProjectGE {
	extern ComPtr<ID3DBlob> CompileShaderForDX12(std::string rawCode, std::string entryFunction, Stages stage, std::string destFile);
};

