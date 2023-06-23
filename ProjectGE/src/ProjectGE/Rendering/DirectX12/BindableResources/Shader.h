#pragma once
#include "gepch.h"
#include "PipelineDefiner.h"
#include <d3dcompiler.h>


namespace ProjectGE {
	class Shader : public PipelineDefiner
	{
	public:
		Shader(const std::string shaderPath, const ShaderType shaderType);
		void Setup(PipelineState& pipeline) override;
	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> m_ShaderBlob;
		ShaderType m_Type;
	};
};


