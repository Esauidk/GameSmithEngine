#pragma once
#include "DirectX12Shader.h"

namespace ProjectGE {
	class DirectX12PixelShader : public DirectX12Shader
	{
	public:
		DirectX12PixelShader(const std::string& shaderPath);
	};

};

