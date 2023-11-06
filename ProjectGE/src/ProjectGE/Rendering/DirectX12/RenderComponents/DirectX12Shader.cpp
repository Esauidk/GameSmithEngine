#include "gepch.h"
#include "DirectX12Shader.h"
#include "ProjectGE/Core/Log.h"
#include "ProjectGE/Rendering/DirectX12/Util/third-party/d3dx12.h"


#pragma comment(lib, "D3DCompiler.lib")

namespace ProjectGE {

	DirectX12Shader::DirectX12Shader(const std::string path) : m_Blob(nullptr) {
		auto pathConvert = std::wstring(path.begin(), path.end());
		bool res = FAILED(D3DReadFileToBlob(pathConvert.c_str(), &m_Blob));
		GE_CORE_ASSERT(!res, "Failed to read shader file");

		GenerateHash();
	}

	DirectX12Shader::DirectX12Shader(ComPtr<ID3DBlob> loadedByteCode) : m_Blob(loadedByteCode){
		GenerateHash();
	}

	DirectX12Shader::DirectX12Shader(char* byteCode, unsigned int length)
	{
		bool res = FAILED(D3DCreateBlob(length, &m_Blob));
		GE_CORE_ASSERT(!res, "Failed to create blob buffer");

		memcpy(m_Blob->GetBufferPointer(), byteCode, length);

		GenerateHash();
	}

	void DirectX12Shader::GenerateHash()
	{
		const char* data = (char*)m_Blob->GetBufferPointer();
		const std::size_t size = m_Blob->GetBufferSize();

		std::hash<std::string> hasher;
		std::string bytecode(data, size);

		m_Hash = hasher(bytecode);
	}
};
