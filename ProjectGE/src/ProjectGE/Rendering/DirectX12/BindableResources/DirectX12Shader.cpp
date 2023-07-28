#include "gepch.h"
#include "DirectX12Shader.h"
#include "ProjectGE/Log.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"


#pragma comment(lib, "D3DCompiler.lib")

namespace ProjectGE {
	DirectX12Shader::DirectX12Shader(const std::string& shaderPath, const ShaderType shaderType) : m_ShaderBlob(nullptr), m_Type(shaderType) {
		auto pathConvert = std::wstring(shaderPath.begin(), shaderPath.end());
		bool res = FAILED(D3DReadFileToBlob(pathConvert.c_str(), &m_ShaderBlob));
		GE_CORE_ASSERT(!res, "Failed to read not read shader file {0}", shaderPath);
	}
	
	void DirectX12Shader::Append(PipelineStateObject& pipeline) {
		auto& dPipeline = (DirectX12PipelineState&)pipeline;
		dPipeline.Attach(m_ShaderBlob.Get(), m_Type);
	}
};
