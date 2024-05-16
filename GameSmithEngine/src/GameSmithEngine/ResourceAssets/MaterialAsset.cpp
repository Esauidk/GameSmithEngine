#include "gepch.h"
#include "MaterialAsset.h"
#include "GameSmithEngine/Core/Log.h"

#include "GameSmithEngine/ResourceAssets/ShaderAsset.h"
#include "GameSmithEngine/ResourceManagement/ResourceManager.h"
#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

namespace GameSmith {
	Ref<char> MaterialAsset::Serialize()
	{
		// TODO: Implement
		return Ref<char>();
	}

	void MaterialAsset::Serialize(Ref<char> byteStream, unsigned int availableBytes)
	{
		// TODO: Implement
	}

	unsigned int MaterialAsset::RequireSpace() const
	{
		// TODO: Implement
		return 0;
	}

	void MaterialAsset::Deserialize(char* inData, unsigned int size)
	{
		ResourceAssetReader reader(inData, size);

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

		m_GlobalVer =  Ref<Material>(new Material(shaders, *config, parameterNames, textureNames, parameterMap, textureMap));
	}

	Ref<Material> MaterialAsset::CreateInstance()
	{
		return Ref<Material>(new Material(*m_GlobalVer));
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
