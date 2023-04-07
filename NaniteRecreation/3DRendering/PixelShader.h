#pragma once
#include "Shader.h"

namespace Render {
	class PixelShader : public Shader
	{
	public:
		PixelShader(const std::wstring shaderPath);
		void Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) override;
	};

};

