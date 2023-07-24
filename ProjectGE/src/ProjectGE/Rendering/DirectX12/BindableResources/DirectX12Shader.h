#pragma once
#include "gepch.h"
#include "DirectX12PipelineDefiner.h"
#include <d3dcompiler.h>


namespace ProjectGE {
	class DirectX12Shader : public DirectX12PipelineDefiner
	{
	public:
		DirectX12Shader(const std::string& shaderPath, const DirectX12ShaderType shaderType);
		void Setup(DirectX12PipelineState& pipeline) override;
	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> m_ShaderBlob;
		DirectX12ShaderType m_Type;
	};
};


