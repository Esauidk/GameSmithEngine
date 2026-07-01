#pragma once

#include "GameSmithEngine/Core/Core.h"
#include <dxcapi.h>
#include <wrl.h>
#include "GameSmithEngine/Rendering/RenderAgnostics/Shaders/ShaderIncludeCache.h"
#include "GameSmithEngine/Rendering/DirectX12/Util/DirectX12ShaderIncludeResolver.h"

using Microsoft::WRL::ComPtr;

namespace GameSmith {
	// Wraps the DirectX Shader Compiler (DXC) for compiling HLSL source to shader
	// bytecode. Supports include resolution via DirectX12ShaderIncludeResolver and
	// configurable debug/optimization flags per build configuration.
	class GE_API DirectX12ShaderCompiler
	{
	public:
		DirectX12ShaderCompiler();

		Ref<const char> CompileShader(
			const char* source,
			const unsigned int sourceLength,
			const std::string entryPoint,
			const std::string profile,
			ShaderIncludeCache* cache,
			unsigned int* outSize
		);
	private:
		ComPtr<IDxcCompiler3> m_DxCompiler;
		ComPtr<IDxcUtils> m_DxUtils;

		ComPtr<DirectX12ShaderIncludeResolver> m_IncludeResolver;
	};
};