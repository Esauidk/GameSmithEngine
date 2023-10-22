#include "gepch.h"
#include "MaterialAsset.h"
#include "ProjectGE/Core/Log.h"

#include "ProjectGE/ResourceAssets/ShaderAsset.h"
#include "ProjectGE/ResourceManagement/ResourceManager.h"
#include "ProjectGE/ResourceManagement/ResourceAssetHelper.h"

namespace ProjectGE {
	void MaterialAsset::Init()
	{
		GE_CORE_INFO("Loading Material");
		m_GlobalVer = MaterialAssetHelper::ReadAsset(GetResourceData(), GetResourceSize());
	}

	void MaterialAsset::Destroy()
	{

	}

	Ref<Material> MaterialAsset::CreateInstance()
	{
		return Ref<Material>(new Material(*m_GlobalVer));
	}


	Ref<Material> MaterialAssetHelper::ReadAsset(char* assetData, unsigned int dataSize)
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
		std::unordered_map<std::string, Ref<ShaderParameter>> parameterMap;
		std::unordered_map<std::string, Ref<TextureAsset>> textureMap;

		for (unsigned int i = 0; i < metadata->ParamterCount; i++) {
			GE_CORE_INFO("Loading parameter {0}", i);
			std::string paramName = reader.GetString();
			ShaderDataType* dataType = reader.ReadClass<ShaderDataType>();
			unsigned int dataSize = GetParameterSize(*dataType);
			char* data = reader.GetBytes(dataSize);

			Ref<ShaderParameter> parm = ConvertToParameter(paramName, *dataType, data);

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
