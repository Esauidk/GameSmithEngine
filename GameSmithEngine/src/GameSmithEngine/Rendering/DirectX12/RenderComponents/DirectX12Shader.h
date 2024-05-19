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

		inline virtual unsigned int GetHash() override { return m_Hash; }


		inline virtual const char* GetRawByteCode() override {
			return (char*)m_Blob->GetBufferPointer();
		};

		inline virtual unsigned int GetByteCodeSize() override { 
			return (unsigned int)m_Blob->GetBufferSize();
		};

	private:
		void GenerateHash();
	private:
		ComPtr<ID3DBlob> m_Blob;
		unsigned int m_Hash;
	};
};


