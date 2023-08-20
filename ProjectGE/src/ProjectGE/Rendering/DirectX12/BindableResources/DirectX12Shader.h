#pragma once
#include "gepch.h"
#include <wrl.h>
#include <d3dcompiler.h>
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/Shader.h"
#include "ProjectGE/Rendering/RenderAgnostics/BindableResources/ConstantBuffer.h"


namespace ProjectGE {
	class DirectX12Shader : public Shader
	{
	public:
		DirectX12Shader(const std::string vertexPath, const std::string pixelPath);
		inline ID3DBlob* GetVertexByteCode() const { return m_VertexBlob.Get(); }
		inline ID3DBlob* GetPixelByteCode() const { return m_PixelBlob.Get(); }
		void Bind() override;
	private:
		Microsoft::WRL::ComPtr<ID3DBlob> m_VertexBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> m_PixelBlob;
		Ref<ConstantBuffer> m_uniformCBuf;
		//Ref<ShaderArguement> m_UnformSlot;

	};
};


