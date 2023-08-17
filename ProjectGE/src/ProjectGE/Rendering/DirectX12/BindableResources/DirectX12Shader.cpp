#include "gepch.h"
#include "DirectX12Shader.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"


#pragma comment(lib, "D3DCompiler.lib")

namespace ProjectGE {
	DirectX12Shader::DirectX12Shader(const std::string vertexPath, const std::string pixelPath, ShaderArguement* shaderUnifromsSlot, UINT inputSize) : m_VertexBlob(nullptr), m_PixelBlob(nullptr), m_UnformSlot(shaderUnifromsSlot) {
		auto pathConvert = std::wstring(vertexPath.begin(), vertexPath.end());
		bool res = FAILED(D3DReadFileToBlob(pathConvert.c_str(), &m_VertexBlob));
		GE_CORE_ASSERT(!res, "Failed to read vertex shader file");

		pathConvert = std::wstring(pixelPath.begin(), pixelPath.end());
		res = FAILED(D3DReadFileToBlob(pathConvert.c_str(), &m_PixelBlob));
		GE_CORE_ASSERT(!res, "Failed to read pixel shader file");

		if (m_UnformSlot != nullptr) {
			m_uniformCBuf = ConstantBuffer::Create(inputSize);
		}
		
	}
	
	void DirectX12Shader::Append(PipelineStateObject& pipeline) {
		auto& dPipeline = (DirectX12PipelineState&)pipeline;
		dPipeline.Attach(m_VertexBlob.Get(), ShaderType::Vertex);
		dPipeline.Attach(m_PixelBlob.Get(), ShaderType::Pixel);
	}
	void DirectX12Shader::UploadShaderInput(BYTE* data)
	{
		GE_CORE_ASSERT(m_UnformSlot != nullptr, "This shader takes no input");
		m_uniformCBuf->UpdateData(data);
	}
	void DirectX12Shader::Bind()
	{
		if (m_UnformSlot != nullptr) {
			m_UnformSlot->SetData(m_uniformCBuf->GetGPUReference());
			m_UnformSlot->Bind();
		}
		
	}
};
