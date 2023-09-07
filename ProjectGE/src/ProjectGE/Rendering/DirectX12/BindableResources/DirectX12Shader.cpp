#include "gepch.h"
#include "DirectX12Shader.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"


#pragma comment(lib, "D3DCompiler.lib")

namespace ProjectGE {
	DirectX12Shader::DirectX12Shader(const std::string vertexPath, const std::string pixelPath) : m_VertexBlob(nullptr), m_PixelBlob(nullptr) {
		auto pathConvert = std::wstring(vertexPath.begin(), vertexPath.end());
		bool res = FAILED(D3DReadFileToBlob(pathConvert.c_str(), &m_VertexBlob));
		GE_CORE_ASSERT(!res, "Failed to read vertex shader file");

		pathConvert = std::wstring(pixelPath.begin(), pixelPath.end());
		res = FAILED(D3DReadFileToBlob(pathConvert.c_str(), &m_PixelBlob));
		GE_CORE_ASSERT(!res, "Failed to read pixel shader file");

		res = FAILED(D3DReflect(m_VertexBlob->GetBufferPointer(), m_VertexBlob->GetBufferSize(), IID_PPV_ARGS(&m_VertexReflect)));
		GE_CORE_ASSERT(!res, "Failed to reflect vertex shader");

		res = FAILED(D3DReflect(m_PixelBlob->GetBufferPointer(), m_PixelBlob->GetBufferSize(), IID_PPV_ARGS(&m_PixelReflect)));
		GE_CORE_ASSERT(!res, "Failed to reflect pixel shader");

		ID3D12ShaderReflectionConstantBuffer* buffer = m_VertexReflect->GetConstantBufferByName("Global");

		ID3D12ShaderReflectionVariable* test = buffer->GetVariableByIndex(0);
		ID3D12ShaderReflectionType* type = test->GetType();
		D3D12_SHADER_TYPE_DESC varDesc;
		type->GetDesc(&varDesc);
		D3D12_SHADER_VARIABLE_DESC desc;
		test->GetDesc(&desc);
		delete buffer;
	}
	
	/*void DirectX12Shader::Append(PipelineStateObject& pipeline) {
		auto& dPipeline = (DirectX12PipelineState&)pipeline;
		dPipeline.Attach(m_VertexBlob.Get(), ShaderType::Vertex);
		dPipeline.Attach(m_PixelBlob.Get(), ShaderType::Pixel);
	}

	void DirectX12Shader::UploadShaderInput(BYTE* data)
	{
		GE_CORE_ASSERT(m_UnformSlot != nullptr, "This shader takes no input");
		m_uniformCBuf->UpdateData(data);
	}

	*/
	void DirectX12Shader::Bind()
	{
		
	}
};
