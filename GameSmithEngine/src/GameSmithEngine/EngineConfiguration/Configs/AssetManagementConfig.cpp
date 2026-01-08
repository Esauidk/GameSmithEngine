#include "gepch.h"
#include "AssetManagementConfig.h"

#define ASSET_DIRECTORY_KEY "AssetDirectory"

namespace GameSmith {
	void AssetManagementConfig::SetAssetDirectory(const std::string& dir)
	{
		m_AssetDirectory = dir;
		json newJson;
		newJson[ASSET_DIRECTORY_KEY] = m_AssetDirectory;
		UpdateJson(newJson);
	}

	void AssetManagementConfig::ExtractJsonData(const json& newJson)
	{
		if (newJson.contains(ASSET_DIRECTORY_KEY)) {
			m_AssetDirectory = newJson.at(ASSET_DIRECTORY_KEY).get<std::string>();
		}
	}
};