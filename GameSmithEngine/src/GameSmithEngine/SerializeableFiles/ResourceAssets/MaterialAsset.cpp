#include "gepch.h"
#include "GameSmithEngine/Core/Log.h"
#include "MaterialAsset.h"

#include "GameSmithEngine/ResourceManagement/AssetManager.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

#include "GameSmithEngine/SerializeableFiles/ResourceAssets/AssetFactory.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/ShaderTypes/ShaderAsset.h"

namespace GameSmith {
	GE_REGISTERASSET(MaterialAsset);

	Ref<char> MaterialAsset::Serialize()
	{
		BinaryStreamWriter writer(RequiredSpace());

		writer.WriteClass<MaterialAssetMetadata>(&m_Metadata);
		// Write Config
		// 
		// Write Shader Paths
		// Write Parameters
		// Write Texture Paths

		MaterialConfig config;
		writer.WriteClass<MaterialConfig>(&config);

		idData shaderID = { 0,0,0,0 };
		if (m_Shader != nullptr) {
			shaderID = m_Shader->GetID().getData();
		}
		writer.WriteClass<idData>(&shaderID);

		// Parameters
		for (const auto& parm : m_ParameterKeys) {
			const auto& entry = m_Parameters.find(parm)->second;
			writer.WriteString(parm);
			auto type = entry->GetType();
			writer.WriteClass<ContainerDataType>(&type);
			writer.WriteByte(entry->GetCharData(), entry->GetSize());
		}


		for (const auto& texture : m_TextureKeys) {
			const auto& entry = m_Textures.find(texture)->second;
			writer.WriteString(texture);
			idData texID = entry->asset->GetID().getData();
			writer.WriteClass<idData>(&texID);
		}
		
		return writer.GetBuffer();
	}

	void MaterialAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
		// TODO: Implement
	}

	unsigned int MaterialAsset::RequiredSpace() const
	{
		unsigned int size = 0;
		size += sizeof(MaterialAssetMetadata);
		size += sizeof(MaterialConfig);
		size += sizeof(idData);

		for (const auto& parm : m_ParameterKeys) {
			const auto& entry = m_Parameters.find(parm)->second;
			size += (unsigned int)(parm.length()+1);
			size += sizeof(ContainerDataType);
			size += entry->GetSize();
		}


		for (const auto& texture : m_TextureKeys) {
			size += (unsigned int)(texture.length()+1);
			size += sizeof(idData);
		}

		return size;
	}

	void MaterialAsset::Deserialize(char* inData, unsigned int size)
	{
		BinaryStreamReader reader(inData, size);

		MaterialAssetMetadata* matMetadata = reader.ReadClass<MaterialAssetMetadata>();
		MaterialConfig* config = reader.ReadClass<MaterialConfig>();

		m_Metadata = *matMetadata;

		auto instance = AssetManager::GetInstance();

		auto shaderId = reader.ReadClass<idData>();
		ID newId(*shaderId);
		if (newId == ID({ 0,0,0,0 })) {
			m_Shader = nullptr;
		}
		else {
			m_Shader = instance->GetResource<ShaderAsset>(newId);
		}

		for (unsigned int i = 0; i < matMetadata->ParamterCount; i++) {
			GE_CORE_INFO("Loading parameter {0}", i);
			std::string paramName = reader.GetString();
			ContainerDataType* dataType = reader.ReadClass<ContainerDataType>();
			unsigned int dataSize = GetParameterSize(*dataType);
			char* data = reader.GetBytes(dataSize);

			Ref<ParameterContainer> param = ConvertToParameter(paramName, *dataType, data);

			m_ParameterKeys.push_back(paramName);
			m_Parameters[paramName] = param;
		}

		for (unsigned int i = 0; i < matMetadata->TetureCount; i++) {
			GE_CORE_INFO("Loading texture {0}", i);

			std::string texName = reader.GetString();
			auto texId = reader.ReadClass<idData>();
			ID newID(*texId);
			Ref<TextureAsset> tex = instance->GetResource<TextureAsset>(newID);

			m_TextureKeys.push_back(texName);
			Ref<TextureRefContainer> container = Ref<TextureRefContainer>(new TextureRefContainer{ tex });
			m_Textures[texName] = container;
		}
	}

	Ref<Material> MaterialAsset::CreateInstance()
	{
		MaterialConfig config;

		std::unordered_map<std::string, Ref<Texture2D>> textureMap;
		for (auto& entry : m_Textures) {
			textureMap[entry.first] = entry.second->asset->GetTexture();
		}

		ShaderSet shaderSet;
		for (int i = 0; i < STAGE_NUM; i++) {
			Stages stage = (Stages)i;
			if (m_Shader->HasShader(stage)) {
				shaderSet.shaders[stage] = m_Shader->GetShader(stage);
			}
			else {
				shaderSet.shaders[stage] = nullptr;
			}
		}

		return Ref<Material>(new Material(shaderSet, config, m_ParameterKeys, m_TextureKeys, m_Parameters, textureMap));
	}

	MaterialAsset::MaterialAsset(std::string name) : Asset(name)
	{
		m_Registry.AddExposedAsset<ShaderAsset>(
			"Shader",
			(Ref<GameSmith::IAsset>*) & (m_Shader),
			CLASS_TO_STRING(ShaderAsset)
		);
	}

	/*MaterialAsset::MaterialAsset(
		const std::string name,
		const std::unordered_map<Stages, ID>& shaderIds,
		const std::vector<std::string>& textureOrder,
		const std::unordered_map<std::string, ID>& textureIds,
		const std::vector<std::string>& parameterOrder,
		const std::unordered_map<std::string, ContainerDataType>& variables
	) : Asset(name), m_TextureKeys(textureOrder), m_ParameterKeys(parameterOrder)
	{
		auto instance = AssetManager::GetInstance();

		for (auto& entry : shaderIds) {
			m_Shaders.shaders[entry.first] = instance->GetResource<ShaderAsset>(entry.second);
			m_Metadata.Shaders[entry.first].UsedShader = true;
			m_Registry.AddExposedAsset<ShaderAsset>(
				std::format("Shader:Stage:{}", entry.first),
				(Ref<GameSmith::IAsset>*) &(m_Shaders.shaders[entry.first]),
				CLASS_TO_STRING(ShaderAsset)
			);
		}

		m_Metadata.TetureCount = (unsigned int)textureIds.size();
		std::vector<std::string> textureNames;
		std::unordered_map<std::string, Ref<Texture2D>> textureMap;
		for (auto& textureEntry : textureIds) {
			Ref<TextureAsset> tex = instance->GetResource<TextureAsset>(textureEntry.second);
			Ref<TextureRefContainer> container = Ref<TextureRefContainer>(new TextureRefContainer{ tex });
			m_Textures[textureEntry.first] = container;
			m_Registry.AddExposedAsset<TextureAsset>(textureEntry.first, (Ref<GameSmith::IAsset>*)&container->asset, CLASS_TO_STRING(TextureAsset));
		}

		m_Metadata.ParamterCount = (unsigned int)variables.size();
		for (auto& entry : variables) {
			auto variableContainor = CreateContainer(entry.first, entry.second);
			m_Parameters[entry.first] = variableContainor;
			m_Registry.AddExposedVariable(entry.first, variableContainor);

		}
	}*/
};
