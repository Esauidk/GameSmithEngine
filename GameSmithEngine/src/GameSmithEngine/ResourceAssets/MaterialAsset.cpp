#include "gepch.h"
#include "MaterialAsset.h"
#include "GameSmithEngine/Core/Log.h"

#include "GameSmithEngine/ResourceAssets/ShaderAsset.h"
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

namespace GameSmith {
	Ref<char> MaterialAsset::Serialize()
	{
		ResourceAssetWriter writer(RequireSpace());

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
				writer.WriteString(m_ShaderPaths[i]);
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


		for (std::string& texture : m_TexturePaths) {
			writer.WriteString(texture);
		}

		writer.CommitToFile("C:\\Users\\esaus\\Documents\\Coding Projects\\GameSmithEngine\\bin\\Debug-windows-x86_64\\TestZone\\TestMat.mat");
		
		// TODO: Implement
		return Ref<char>();
	}

	void MaterialAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
		// TODO: Implement
	}

	unsigned int MaterialAsset::RequireSpace() const
	{
		// TODO: Implement
		return 700;
	}

	void MaterialAsset::Deserialize(char* inData, unsigned int size)
	{
		ResourceAssetReader reader(inData, size);

		MaterialAssetMetadata* metadata = reader.ReadClass<MaterialAssetMetadata>();
		MaterialConfig* config = reader.ReadClass<MaterialConfig>();

		m_Metadata = *metadata;

		auto instance = ResourceManager::GetInstance();

		ShaderSet shaders;
		for (unsigned int i = 0; i < STAGE_NUM; i++) {
			Stages stage = (Stages)i;
			if (metadata->Shaders[i].UsedShader) {
				auto shaderPath = reader.GetString();
				shaders.shaders[i] = instance->GetResource<ShaderAsset>(shaderPath);
				m_ShaderPaths[i] = shaderPath;
			}
		}

		std::vector<std::string> parameterNames;
		std::vector<std::string> textureNames;
		std::unordered_map<std::string, Ref<ParameterContainer>> parameterMap;
		std::unordered_map<std::string, Ref<TextureAsset>> textureMap;

		for (unsigned int i = 0; i < metadata->ParamterCount; i++) {
			GE_CORE_INFO("Loading parameter {0}", i);
			std::string paramName = reader.GetString();
			ContainerDataType* dataType = reader.ReadClass<ContainerDataType>();
			unsigned int dataSize = GetParameterSize(*dataType);
			char* data = reader.GetBytes(dataSize);

			Ref<ParameterContainer> parm = ConvertToParameter(paramName, *dataType, data);

			parameterNames.push_back(paramName);
			parameterMap.insert({ paramName, parm });
		}

		for (unsigned int i = 0; i < metadata->TetureCount; i++) {
			GE_CORE_INFO("Loading texture {0}", i);

			std::string texName = reader.GetString();
			std::string texPath = reader.GetString();
			m_TexturePaths.push_back(texPath);
			Ref<TextureAsset> tex = instance->GetResource<TextureAsset>(texPath);

			textureNames.push_back(texName);
			textureMap.insert({ texName, tex });
		}

		m_GlobalVer =  Ref<Material>(new Material(shaders, *config, parameterNames, textureNames, parameterMap, textureMap));
	}

	Ref<Material> MaterialAsset::CreateInstance()
	{
		return Ref<Material>(new Material(*m_GlobalVer));
	}

	MaterialAsset::MaterialAsset(
		std::vector<std::pair<std::string, Stages>>& shaderPaths, 
		std::vector<std::pair<std::string, std::string>>& texturePaths,
		std::vector<std::pair<std::string, ContainerDataType>>& variables)
	{
		GE_CORE_ASSERT(shaderPaths.size() <= STAGE_NUM, "More shader entries than there are stages");

		auto instance = ResourceManager::GetInstance();

		ShaderSet shaders;
		for (auto& entry : shaderPaths) {
			GE_CORE_ASSERT(shaders.shaders[entry.second] == nullptr, "ShaderPath parameter passed has a double entry for a stage");

			shaders.shaders[entry.second] = instance->GetResource<ShaderAsset>(entry.first);
			m_ShaderPaths[entry.second] = entry.first;

			m_Metadata.Shaders[entry.second].UsedShader = true;
		}

		m_Metadata.TetureCount = (unsigned int)texturePaths.size();
		std::vector<std::string> textureNames;
		std::unordered_map<std::string, Ref<TextureAsset>> textureMap;
		for (auto& textureEntry : texturePaths) {
			m_TexturePaths.push_back(textureEntry.second);
			Ref<TextureAsset> tex = instance->GetResource<TextureAsset>(textureEntry.second);
			textureNames.push_back(textureEntry.first);
			textureMap.insert({ textureEntry.first, tex });
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

	Ref<Material> MaterialAsset::ReadAsset(char* assetData, unsigned int dataSize)
	{
		ResourceAssetReader reader(assetData, dataSize);

		MaterialAssetMetadata* metadata = reader.ReadClass<MaterialAssetMetadata>();
		MaterialConfig* config = reader.ReadClass<MaterialConfig>();

		auto instance = ResourceManager::GetInstance();

		ShaderSet shaders;
		for (unsigned int i = 0; i < STAGE_NUM; i++) {
			Stages stage = (Stages)i;
			if (metadata->Shaders[i].UsedShader) {
				shaders.shaders[i] = instance->GetResource<ShaderAsset>(reader.GetString());
			}
		}
		
		std::vector<std::string> parameterNames;
		std::vector<std::string> textureNames;
		std::unordered_map<std::string, Ref<ParameterContainer>> parameterMap;
		std::unordered_map<std::string, Ref<TextureAsset>> textureMap;

		for (unsigned int i = 0; i < metadata->ParamterCount; i++) {
			GE_CORE_INFO("Loading parameter {0}", i);
			std::string paramName = reader.GetString();
			ContainerDataType* dataType = reader.ReadClass<ContainerDataType>();
			unsigned int dataSize = GetParameterSize(*dataType);
			char* data = reader.GetBytes(dataSize);

			Ref<ParameterContainer> parm = ConvertToParameter(paramName, *dataType, data);

			parameterNames.push_back(paramName);
			parameterMap.insert({ paramName, parm });
		}

		for (unsigned int i = 0; i < metadata->TetureCount; i++) {
			GE_CORE_INFO("Loading texture {0}", i);

			std::string texName = reader.GetString();
			Ref<TextureAsset> tex = instance->GetResource<TextureAsset>(reader.GetString());

			textureNames.push_back(texName);
			textureMap.insert({ texName, tex });
		}

		return Ref<Material>(new Material(shaders, *config, parameterNames, textureNames, parameterMap, textureMap));
	}
};
