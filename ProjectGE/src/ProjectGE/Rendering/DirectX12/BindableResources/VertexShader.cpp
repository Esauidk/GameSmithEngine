#include "gepch.h"
#include "VertexShader.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"

namespace ProjectGE {
	VertexShader::VertexShader(const std::string shaderPath): Shader(shaderPath, VERTEX){}
};
