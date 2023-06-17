#include "VertexShader.h"
#include "../../../Util/DirectX12/d3dx12.h"

namespace Render {
	VertexShader::VertexShader(const std::wstring shaderPath): Shader(shaderPath, VERTEX){}

	//TODO: IMplement Binding
	void VertexShader::Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) {
	
	}
};
