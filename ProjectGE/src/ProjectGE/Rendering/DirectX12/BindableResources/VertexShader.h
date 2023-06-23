#pragma once
#include "Shader.h"

namespace ProjectGE {
	class VertexShader : public Shader
	{
	public:
		VertexShader(const std::string shaderPath);
		void Bind(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> cmdList) override;
	};

};

