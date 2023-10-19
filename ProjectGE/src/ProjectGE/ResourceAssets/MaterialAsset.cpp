#include "gepch.h"
#include "MaterialAsset.h"
#include "ProjectGE/Core/Log.h"

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

		MaterialAssetMetadata* metadata = reader.ReadData<MaterialAssetMetadata>();
		MaterialConfig* config = reader.ReadData<MaterialConfig>();

		ShaderSet shaders;
		std::vector<std::string> parameterNames;
		std::vector<std::string> textureNames;
		std::unordered_map<std::string, Ref<ShaderParameter>> parameterMap;
		std::unordered_map<std::string, Ref<TextureAsset>> textureMap;


		for (unsigned int i = 0; i < metadata->ParamterCount; i++) {
			GE_CORE_INFO("Loading parameter {0}", i);
		}

		for (unsigned int i = 0; i < metadata->TetureCount; i++) {
			GE_CORE_INFO("Loading texture {0}", i);
		}
		
		return Ref<Material>(new Material(shaders, *config, parameterNames, textureNames, parameterMap, textureMap));
	}
};
