#include "gepch.h"
#include "Shader.h"
#include "ProjectGE/Log.h"
#include "ProjectGE/Rendering/DirectX12/Util/d3dx12.h"


#pragma comment(lib, "D3DCompiler.lib")

namespace ProjectGE {
	Shader::Shader(const std::string& shaderPath, const ShaderType shaderType) : m_ShaderBlob(nullptr), m_Type(shaderType) {
		auto pathConvert = std::wstring(shaderPath.begin(), shaderPath.end());
		bool res = FAILED(D3DReadFileToBlob(pathConvert.c_str(), &m_ShaderBlob));
		GE_CORE_ASSERT(!res, "Failed to read not read shader file {0}", shaderPath);
	}
	
	void Shader::Setup(PipelineState& pipeline) {
		pipeline.Attach(m_ShaderBlob.Get(), m_Type);
	}
};
