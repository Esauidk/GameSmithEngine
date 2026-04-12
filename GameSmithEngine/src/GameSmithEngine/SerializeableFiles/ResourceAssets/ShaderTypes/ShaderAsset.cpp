#include "gepch.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"
#include "ShaderAsset.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"
#include "GameSmithEngine/ResourceManagement/AssetManager.h"

namespace GameSmith {
	Ref<char> ShaderAsset::Serialize()
	{
		BinaryStreamWriter writer(RequiredSpace());
		idData sourceID = m_HLSLSource->GetID().getData();
		writer.WriteClass<idData>(&sourceID);

		return writer.GetBuffer();
	}
	void ShaderAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
		auto requiredSize = RequiredSpace();

		GE_CORE_ASSERT(
			requiredSize <= availableBytes, 
			"The given byte array (given: {0}) is not big enough to serialize this shader (required: {1})", 
			availableBytes, requiredSize);

		BinaryStreamWriter writer(byteStream, availableBytes);
		idData sourceID = m_HLSLSource->GetID().getData();
		writer.WriteClass<idData>(&sourceID);
	}

	unsigned int ShaderAsset::RequiredSpace() const
	{
		unsigned int requiredSize = sizeof(idData);;

		return requiredSize;
	}

	void ShaderAsset::Deserialize(char* inData, unsigned int size)
	{
		BinaryStreamReader reader(inData, size);
		idData sourceID = *reader.ReadClass<idData>();
		ID sourceFullID(sourceID);

		auto assetManager = AssetManager::GetInstance();
		m_HLSLSource = assetManager->GetResource<HLSLAsset>(sourceFullID);
	}

	Ref<Shader> ShaderAsset::GetShader()
	{
		if (m_Shader == nullptr) {
			auto renderManager = RenderingManager::GetInstance();
			const ID assetID = GetID();
			m_Shader = renderManager->CompileOrRetrieveShader(STAGE_VERTEX, m_HLSLSource->GetBytes(), m_HLSLSource->GetSize(), assetID);
		}

		return m_Shader;
	}
};

