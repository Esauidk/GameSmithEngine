#pragma once
#include "gepch.h"
#include <wrl.h>
#include <d3dcompiler.h>
#include "ProjectGE/Rendering/RenderAgnostics/RenderComponents/Shader.h"

using Microsoft::WRL::ComPtr;

namespace ProjectGE {
	class DirectX12Shader : public Shader
	{
	public:
		DirectX12Shader(const std::string path);
		DirectX12Shader(ComPtr<ID3DBlob> loadedByteCode);
		inline ID3DBlob* ByteCode() const { return m_Blob.Get(); }
		void Bind() override;
	private:
		ComPtr<ID3DBlob> m_Blob;
	};
};


