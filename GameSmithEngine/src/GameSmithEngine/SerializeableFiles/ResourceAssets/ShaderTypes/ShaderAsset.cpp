#include "gepch.h"
#include "GameSmithEngine/Core/Log.h"
#include "GameSmithEngine/Rendering/RenderingManager.h"
#include "GameSmithEngine/ResourceManagement/AssetManager.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/AssetFactory.h"
#include "ShaderAsset.h"

namespace GameSmith {
	GE_REGISTERASSET(ShaderAsset);

	Ref<char> ShaderAsset::Serialize()
	{
		BinaryStreamWriter writer(RequiredSpace());
		for (unsigned int i = 0; i < STAGE_NUM; i++) {
			if (m_HLSLSource.sources[i] != nullptr) {
				idData sourceID = m_HLSLSource.sources[i]->GetID().getData();
				writer.WriteClass<idData>(&sourceID);
			}
			else {
				idData emptyID = { 0, 0, 0, 0 };
				writer.WriteClass<idData>(&emptyID);
			}
		}

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
		for (unsigned int i = 0; i < STAGE_NUM; i++) {
			if (m_HLSLSource.sources[i] != nullptr) {
				idData sourceID = m_HLSLSource.sources[i]->GetID().getData();
				writer.WriteClass<idData>(&sourceID);
			}
			else {
				idData emptyID = { 0, 0, 0, 0 };
				writer.WriteClass<idData>(&emptyID);
			}
		}
	}

	unsigned int ShaderAsset::RequiredSpace() const
	{
		unsigned int requiredSize = sizeof(idData) * STAGE_NUM;

		return requiredSize;
	}

	void ShaderAsset::Deserialize(char* inData, unsigned int size)
	{
		BinaryStreamReader reader(inData, size);
		auto assetManager = AssetManager::GetInstance();
		for (unsigned int i = 0; i < STAGE_NUM; i++) {
			idData sourceID = *reader.ReadClass<idData>();
			ID sourceFullID(sourceID);
			if (sourceFullID == ID()) {
				continue;
			}

			m_HLSLSource.sources[i] = assetManager->GetResource<HLSLAsset>(sourceFullID);
		}
	}

	bool ShaderAsset::HasShader(const Stages stage) const
	{
		return m_HLSLSource.sources[stage] != nullptr;
	}

	Ref<Shader> ShaderAsset::GetShader(const Stages stage)
	{
		if (m_Shaders[stage] == nullptr) {
			GE_CORE_ASSERT(m_HLSLSource.sources[stage] != nullptr, "Shader stage {0} does not have a source to compile from", ConvertShaderStageToString(stage));

			auto renderManager = RenderingManager::GetInstance();
			const ID assetID = GetID();
			m_Shaders[stage] = renderManager->CompileOrRetrieveShader(stage, m_HLSLSource.sources[stage]->GetBytes(), m_HLSLSource.sources[stage]->GetSize(), assetID);
		}

		return m_Shaders[stage];
	}
};

