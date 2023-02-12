#include "Shader.h"
#include "DirectXMacros.h"

#pragma comment(lib, "D3DCompiler.lib")

namespace Render {
	Shader::Shader(const std::wstring shaderPath) {
		RENDER_THROW(D3DReadFileToBlob(shaderPath.c_str(), &shaderBlob));
	}
};
