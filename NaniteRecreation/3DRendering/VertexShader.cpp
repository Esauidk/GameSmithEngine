#include "VertexShader.h"
#include "d3dx12.h"

namespace Render {
	VertexShader::VertexShader(const std::wstring shaderPath): Shader(shaderPath, VERTEX){}

	//TODO: IMplement Binding
	void VertexShader::Bind() {
	
	}
};
