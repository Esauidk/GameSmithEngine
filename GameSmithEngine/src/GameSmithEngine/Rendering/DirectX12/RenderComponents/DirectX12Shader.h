#pragma once
#include "gepch.h"
#include <wrl.h>
#include <d3dcompiler.h>
#include "GameSmithEngine/Rendering/RenderAgnostics/RenderComponents/Shader.h"

using Microsoft::WRL::ComPtr;

namespace GameSmith {
	class DirectX12Shader : public Shader
	{
	public:
		DirectX12Shader(const std::string path);
		DirectX12Shader(ComPtr<ID3DBlob> loadedByteCode);
		DirectX12Shader(char* byteCode, unsigned int length);
		inline ID3DBlob* ByteCode() const { return m_Blob.Get(); }
		inline virtual Stages GetShaderType() override {
			// TODO: Return actual shader type
			return Stages::STAGE_NUM; 
		}

		virtual unsigned int GetHash() { return m_Hash; }
	private:
		void GenerateHash();
	private:
		ComPtr<ID3DBlob> m_Blob;
		unsigned int m_Hash;
	};
};


