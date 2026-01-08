#include "gepch.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"
#include "ShaderAsset.h"

namespace GameSmith {

	Ref<char> ShaderAsset::Serialize()
	{
		auto byteArray = Ref<char>(new char[RequiredSpace()]);

		memcpy(byteArray.get(), m_Shader->GetRawByteCode(), RequiredSpace());

		return byteArray;
	}
	void ShaderAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
		// TODO: Implement
		auto requiredSize = RequiredSpace();

		GE_CORE_ASSERT(
			requiredSize <= availableBytes, 
			"The given byte array (given: {0}) is not big enough to serialize this shader (required: {1})", 
			availableBytes, requiredSize);

		memcpy(byteStream, m_Shader->GetRawByteCode(), RequiredSpace());
	}

	unsigned int ShaderAsset::RequiredSpace() const
	{
		return m_Shader->GetByteCodeSize();
	}

	void ShaderAsset::Deserialize(char* inData, unsigned int size)
	{
		auto renderAPI = RenderingManager::GetInstance()->GetRenderAPI();
		m_Shader = renderAPI->LoadShader(inData, size);
	}
};

