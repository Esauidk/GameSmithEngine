#include "gepch.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"
#include "ShaderAsset.h"

namespace GameSmith {
	inline ShaderAsset::ShaderAsset(std::string name, const char* data, unsigned int dataSize) : Asset(name)
	{
		m_SerializedData = Ref<char>(new char[dataSize]);
		memcpy(&m_SerializedData, data, dataSize);

		m_SerializedDataSize = dataSize;
	}

	Ref<char> ShaderAsset::Serialize()
	{
		auto byteArray = Ref<char>(new char[RequiredSpace()]);

		memcpy(byteArray.get(), m_Shader->GetRawByteCode(), RequiredSpace());

		return byteArray;
	}
	void ShaderAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
		auto requiredSize = RequiredSpace();

		GE_CORE_ASSERT(
			requiredSize <= availableBytes, 
			"The given byte array (given: {0}) is not big enough to serialize this shader (required: {1})", 
			availableBytes, requiredSize);

 		memcpy(byteStream, m_SerializedData.get(), RequiredSpace());
	}

	unsigned int ShaderAsset::RequiredSpace() const
	{
		return m_SerializedDataSize;
	}

	void ShaderAsset::Deserialize(char* inData, unsigned int size)
	{
		m_SerializedData = Ref<char>(new char[size]);
		memcpy(m_SerializedData.get(), inData, size);
		m_SerializedDataSize = size;

		auto renderAPI = RenderingManager::GetInstance()->GetRenderAPI();
		m_Shader = renderAPI->LoadShader(inData, size);
	}

	Ref<Shader> ShaderAsset::GetShader()
	{
		if (m_Shader == nullptr) {
			auto renderManager = RenderingManager::GetInstance();
			const ID assetID = GetID();
			m_Shader = renderManager->CompileOrRetrieveShader(STAGE_VERTEX, m_SerializedData.get(), m_SerializedDataSize, assetID);
		}

		return m_Shader;
	}
};

