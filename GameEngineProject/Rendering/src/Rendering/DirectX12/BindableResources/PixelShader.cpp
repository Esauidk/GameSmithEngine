#include "PixelShader.h"
#include "../../../Util/DirectX12/d3dx12.h"

namespace Render {
	PixelShader::PixelShader(const std::wstring shaderPath): Shader(shaderPath, PIXEL) {}

	//TODO: IMplement Binding
	void PixelShader::Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) {
		
	}
};
