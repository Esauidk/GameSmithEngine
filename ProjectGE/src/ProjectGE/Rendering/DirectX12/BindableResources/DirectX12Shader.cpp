#include "gepch.h"
#include "DirectX12Shader.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"


#pragma comment(lib, "D3DCompiler.lib")

namespace ProjectGE {
	static ParameterType TranslateVariableTypeReflection(D3D12_SHADER_TYPE_DESC type) {
		switch (type.Class) {
		case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_MATRIX_COLUMNS:
		case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_MATRIX_ROWS:
			return ParameterType::MATRIX;
		case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_VECTOR:
			switch (type.Rows + type.Columns) {
			case 2:
				return ParameterType::FLOAT2;
			case 3:
				return ParameterType::FLOAT3;
			case 4:
				return ParameterType::FLOAT4;
			default:
				return ParameterType::FLOAT;
			}
		}
	}

	DirectX12Shader::DirectX12Shader(const std::string path) : m_Blob(nullptr) {
		auto pathConvert = std::wstring(path.begin(), path.end());
		bool res = FAILED(D3DReadFileToBlob(pathConvert.c_str(), &m_Blob));
		GE_CORE_ASSERT(!res, "Failed to read vertex shader file");

		res = FAILED(D3DReflect(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), IID_PPV_ARGS(&m_Reflect)));
		GE_CORE_ASSERT(!res, "Failed to reflect vertex shader");

		InspectShader();
		res;
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

	void DirectX12Shader::InspectShader()
	{
		D3D12_SHADER_DESC shaderDesc;
		m_Reflect->GetDesc(&shaderDesc);

		// Examine all constant buffers
		for (UINT i = 0; i < shaderDesc.ConstantBuffers; i++) {
			ShaderParameterGroupBuilder builder;
			builder.SetGroupType(GroupType::CBUFFER);
			builder.SetRegisterSlot(i);

			D3D12_SHADER_BUFFER_DESC cBufferDesc;
			ID3D12ShaderReflectionConstantBuffer* buffer = m_Reflect->GetConstantBufferByIndex(i);
			buffer->GetDesc(&cBufferDesc);
			std::string name = cBufferDesc.Name;
			builder.SetGroupName(name);

			for (UINT j = 0; j < cBufferDesc.Variables; j++) {
				ID3D12ShaderReflectionVariable* var = buffer->GetVariableByIndex(j);
				D3D12_SHADER_VARIABLE_DESC varDesc;
				D3D12_SHADER_TYPE_DESC varTypeDesc;

				var->GetDesc(&varDesc);
				ID3D12ShaderReflectionType* varType = var->GetType();

				std::string varName = varDesc.Name;
				varType->GetDesc(&varTypeDesc);
				ParameterType pType = TranslateVariableTypeReflection(varTypeDesc);
				builder.AddParameter(varName, pType);
			}

			m_GroupMapping.emplace(name, builder.GetGroup());
		}

		// Examine all resources
		for (UINT i = 0; i < shaderDesc.BoundResources; i++) {

		}
	}

	
};
