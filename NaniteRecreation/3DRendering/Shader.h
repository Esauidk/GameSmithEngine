#pragma once
#include "PipelineDefiner.h"
#include <string>
#include <wrl.h>
#include <d3dcompiler.h>


namespace Render {
	class Shader : public PipelineDefiner
	{
	public:
		Shader(const std::wstring shaderPath, const ShaderType shaderType);
		void Setup(PipelineState pipeline) override;
	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
		ShaderType type;
	};
};


