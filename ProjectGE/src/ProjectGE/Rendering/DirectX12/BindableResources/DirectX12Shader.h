#pragma once
#include "gepch.h"
#include <d3dcompiler.h>
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Shader.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ConstantBuffer.h"


namespace ProjectGE {
	class DirectX12Shader : public Shader
	{
	public:
		DirectX12Shader(const std::string vertexPath, const std::string pixelPath, ShaderArguement* shaderUnifromsSlot, UINT inputSize = 0);
		void Append(PipelineStateObject& pipeline) override;
		void UploadShaderInput(BYTE* data);
		void Bind() override;
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> m_VertexBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> m_PixelBlob;
		ConstantBuffer* m_uniformCBuf;
		ShaderArguement* m_UnformSlot;

	};
};


