#pragma once
#include "Shader.h"

namespace Render {
	class VertexShader : public Shader
	{
	public:
		VertexShader(const std::wstring shaderPath);
		void Bind() override;
	};

};

