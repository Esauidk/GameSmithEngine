#include "gepch.h"
#include "DirectX12PixelShader.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"

namespace ProjectGE {
	DirectX12PixelShader::DirectX12PixelShader(const std::string& shaderPath): DirectX12Shader(shaderPath, PIXEL) {}
};
