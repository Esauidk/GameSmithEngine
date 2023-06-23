#include "gepch.h"
#include "PixelShader.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"

namespace ProjectGE {
	PixelShader::PixelShader(const std::string shaderPath): Shader(shaderPath, PIXEL) {}

	//TODO: IMplement Binding
	void PixelShader::Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) {
		
	}
};
