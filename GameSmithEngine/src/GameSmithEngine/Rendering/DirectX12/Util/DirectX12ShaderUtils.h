#pragma once

#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Shader.h"
#include <wrl.h>
#include <d3dcompiler.h>

#define SHADER_STATE_STREAM_NULL_SAFE(x, StreamType) x != nullptr ? \
CD3DX12_SHADER_BYTECODE(CastPtr<DirectX12Shader>(x->GetShader())->ByteCode()) : StreamType()

using Microsoft::WRL::ComPtr;
namespace GameSmith {
	extern ComPtr<ID3DBlob> CompileShaderForDX12(std::string rawCode, std::string entryFunction, Stages stage, std::string destFile);
};

