#include "gepch.h"
#include "DirectX12Shader.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"
#include "ProjectGE/Rendering/DirectX12/DirectX12PipelineState.h"


#pragma comment(lib, "D3DCompiler.lib")

namespace ProjectGE {

	DirectX12Shader::DirectX12Shader(const std::string path) : m_Blob(nullptr) {
		auto pathConvert = std::wstring(path.begin(), path.end());
		bool res = FAILED(D3DReadFileToBlob(pathConvert.c_str(), &m_Blob));
		GE_CORE_ASSERT(!res, "Failed to read shader file");
	}

	DirectX12Shader::DirectX12Shader(ComPtr<ID3DBlob> loadedByteCode) : m_Blob(loadedByteCode)
	{
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
