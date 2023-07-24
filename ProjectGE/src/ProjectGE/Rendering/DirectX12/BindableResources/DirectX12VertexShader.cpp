#include "gepch.h"
#include "DirectX12VertexShader.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"

namespace ProjectGE {
	DirectX12VertexShader::DirectX12VertexShader(const std::string shaderPath): DirectX12Shader(shaderPath, VERTEX){}
};
