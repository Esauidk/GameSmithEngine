#pragma once
#include "Shader.h"

namespace Render {
	class PixelShader : public Shader
	{
	public:
		PixelShader(const std::wstring shaderPath);
		void Bind() override;
	};

};

