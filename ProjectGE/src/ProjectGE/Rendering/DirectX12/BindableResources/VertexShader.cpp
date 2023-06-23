#include "gepch.h"
#include "VertexShader.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"

namespace ProjectGE {
	VertexShader::VertexShader(const std::string shaderPath): Shader(shaderPath, VERTEX){}

	//TODO: IMplement Binding
	void VertexShader::Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) {
	
	}
};
