#pragma once
#include "Shader.h"

namespace ProjectGE {
	class PixelShader : public Shader
	{
	public:
		PixelShader(const std::string& shaderPath);
		void Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) override;
	};

};

