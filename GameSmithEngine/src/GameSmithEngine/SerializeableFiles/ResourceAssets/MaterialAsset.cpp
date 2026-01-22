#include "gepch.h"
#include "GameSmithEngine/Core/Log.h"
#include "MaterialAsset.h"

#include "GameSmithEngine/ResourceManagement/AssetManager.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

#include "GameSmithEngine/SerializeableFiles/ResourceAssets/AssetFactory.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/ShaderAsset.h"
#include "GameSmithEngine/SerializeableFiles/ResourceAssets/TextureAsset.h"

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
		//writer.WriteClass<MaterialConfig>(&m_)

		// TODO: Remove this
		MaterialConfig config;
		writer.WriteClass<MaterialConfig>(&config);

		for (unsigned int i = 0; i < STAGE_NUM; i++) {
			if (m_Metadata.Shaders[i].UsedShader) {
				idData shaderID = m_ShaderIds[i].getData();
				writer.WriteClass<idData>(&shaderID);
			}
		}

		// Parameters
		auto& map = m_GlobalVer->DumpCurrentParameterMap();
		auto& order = m_GlobalVer->DumpParameterOrder();

		for (const std::string& parm : order) {
			const auto& entry = map.find(parm)->second;
			writer.WriteString(parm);
			auto type = entry->GetType();
			writer.WriteClass<ContainerDataType>(&type);
			writer.WriteByte(entry->GetCharData(), entry->GetSize());
		}


		for (auto& texture : m_MatInfo->TextureIds) {
			writer.WriteString(texture.first);
			idData texID = texture.second.getData();
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
		// TODO: Implement
		unsigned int size = 0;
		size += sizeof(MaterialAssetMetadata);
		size += sizeof(MaterialConfig);
		for (unsigned int i = 0; i < STAGE_NUM; i++) {
			if (m_Metadata.Shaders[i].UsedShader) {
				size += sizeof(idData);
			}
		}

		auto& map = m_GlobalVer->DumpCurrentParameterMap();
		auto& order = m_GlobalVer->DumpParameterOrder();

		for (const std::string& parm : order) {
			const auto& entry = map.find(parm)->second;
			size += (unsigned int)(parm.length()+1);
			size += sizeof(ContainerDataType);
			size += entry->GetSize();
		}


		for (auto& texture : m_MatInfo->TextureIds) {
			size += (unsigned int)(texture.first.length()+1);
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

		ShaderSet shaders;
		for (unsigned int i = 0; i < STAGE_NUM; i++) {
			Stages stage = (Stages)i;
			if (matMetadata->Shaders[i].UsedShader) {
				auto shaderId = reader.ReadClass<idData>();
				ID newId(*shaderId);
				shaders.shaders[i] = instance->GetResource<ShaderAsset>(newId);
				m_ShaderIds[i] = newId;
			}
		}

		std::vector<std::string> parameterNames;
		std::vector<std::string> textureNames;
		std::unordered_map<std::string, Ref<ParameterContainer>> parameterMap;
		std::unordered_map<std::string, Ref<Texture2D>> textureMap;

		for (unsigned int i = 0; i < matMetadata->ParamterCount; i++) {
			GE_CORE_INFO("Loading parameter {0}", i);
			std::string paramName = reader.GetString();
			ContainerDataType* dataType = reader.ReadClass<ContainerDataType>();
			unsigned int dataSize = GetParameterSize(*dataType);
			char* data = reader.GetBytes(dataSize);

			Ref<ParameterContainer> parm = ConvertToParameter(paramName, *dataType, data);

			parameterNames.push_back(paramName);
			parameterMap.insert({ paramName, parm });
		}

		for (unsigned int i = 0; i < matMetadata->TetureCount; i++) {
			GE_CORE_INFO("Loading texture {0}", i);

			std::string texName = reader.GetString();
			auto texId = reader.ReadClass<idData>();
			ID newID(*texId);
			m_MatInfo->TextureIds.push_back({ texName, newID });
			Ref<TextureAsset> tex = instance->GetResource<TextureAsset>(newID);

			textureNames.push_back(texName);
			textureMap.insert({ texName, tex->GetTexture() });
		}

		m_GlobalVer =  Ref<Material>(new Material(shaders, *config, parameterNames, textureNames, parameterMap, textureMap));
	}

	Ref<Material> MaterialAsset::CreateInstance()
	{
		return Ref<Material>(new Material(*m_GlobalVer));
	}

	MaterialAsset::MaterialAsset(std::string name) : Asset(name), m_MatInfo(new MaterialInfo())
	{
	}

	MaterialAsset::MaterialAsset(
		std::string name,
		std::vector<std::pair<ID, Stages>>& shaderIds,
		std::vector<std::pair<std::string, ID>>& textureIds,
		std::vector<std::pair<std::string, ContainerDataType>>& variables
	) : Asset(name), m_MatInfo(new MaterialInfo())
	{
		GE_CORE_ASSERT(shaderIds.size() <= STAGE_NUM, "More shader entries than there are stages");

		auto instance = AssetManager::GetInstance();

		ShaderSet shaders;
		for (auto& entry : shaderIds) {
			GE_CORE_ASSERT(shaders.shaders[entry.second] == nullptr, "ShaderPath parameter passed has a double entry for a stage");

			shaders.shaders[entry.second] = instance->GetResource<ShaderAsset>(entry.first);
			m_ShaderIds[entry.second] = entry.first;

			m_Metadata.Shaders[entry.second].UsedShader = true;
		}

		m_Metadata.TetureCount = (unsigned int)textureIds.size();
		std::vector<std::string> textureNames;
		std::unordered_map<std::string, Ref<Texture2D>> textureMap;
		for (auto& textureEntry : textureIds) {
			m_MatInfo->TextureIds.push_back({ textureEntry.first, textureEntry.second });
			Ref<TextureAsset> tex = instance->GetResource<TextureAsset>(textureEntry.second);
			textureNames.push_back(textureEntry.first);
			textureMap.insert({ textureEntry.first, tex->GetTexture() });
		}

		m_Metadata.ParamterCount = (unsigned int)variables.size();
		std::vector<std::string> parameterNames;
		std::unordered_map<std::string, Ref<ParameterContainer>> parameterMap;
		for (auto& entry : variables) {
			auto variableContainor = CreateContainer(entry.first, entry.second);
			parameterNames.push_back(entry.first);
			parameterMap.insert({ entry.first, variableContainor });
		}

		MaterialConfig emptyConfig;
		m_GlobalVer = Ref<Material>(new Material(shaders, emptyConfig, parameterNames, textureNames, parameterMap, textureMap));
	}
};
