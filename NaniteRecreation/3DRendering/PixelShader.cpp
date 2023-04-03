#include "PixelShader.h"
#include "d3dx12.h"

namespace Render {
	PixelShader::PixelShader(const std::wstring shaderPath): Shader(shaderPath, PIXEL) {}

	//TODO: IMplement Binding
	void PixelShader::Bind() {
		
	}
};
