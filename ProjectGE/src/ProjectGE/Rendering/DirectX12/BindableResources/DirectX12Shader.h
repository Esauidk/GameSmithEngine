#pragma once
#include "gepch.h"
#include <d3dcompiler.h>
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/PipelineDefiner.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"


namespace ProjectGE {
	class DirectX12Shader : public PipelineDefiner
	{
	public:
		DirectX12Shader(const std::string& shaderPath, const DirectX12ShaderType shaderType);
		void Append(PipelineStateObject& pipeline) override;
	protected:
		Microsoft::WRL::ComPtr<ID3DBlob> m_ShaderBlob;
		DirectX12ShaderType m_Type;
	};
};


