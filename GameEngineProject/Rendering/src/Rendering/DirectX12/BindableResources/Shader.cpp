#include "Shader.h"
#include "../DirectXMacros.h"
#include "../../../Util/DirectX12/d3dx12.h"


#pragma comment(lib, "D3DCompiler.lib")

namespace Render {
	Shader::Shader(const std::wstring shaderPath, const ShaderType shaderType) : shaderBlob(nullptr), type(shaderType) {
		RENDER_THROW(D3DReadFileToBlob(shaderPath.c_str(), &shaderBlob));
	}
	
	void Shader::Setup(PipelineState& pipeline) {
		pipeline.Attach(shaderBlob.Get(), type);
	}
};
