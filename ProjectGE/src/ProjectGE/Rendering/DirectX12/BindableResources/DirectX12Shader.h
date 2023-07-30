#pragma once
#include "gepch.h"
#include <d3dcompiler.h>
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Shader.h"


namespace ProjectGE {
	class DirectX12Shader : public Shader
	{
	public:
		DirectX12Shader(const std::string& shaderPath, const ShaderType shaderType);
		void Append(PipelineStateObject& pipeline) override;
		void Bind(ID3D12GraphicsCommandList6* cmdList) override;
		Microsoft::WRL::ComPtr<ID3DBlob> m_ShaderBlob;
		ShaderType m_Type;
	};
};


