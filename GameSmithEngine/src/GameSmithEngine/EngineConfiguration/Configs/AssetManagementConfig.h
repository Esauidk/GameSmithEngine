#pragma once

#include "GameSmithEngine/SerializeableFiles/GenericFileTypes/JsonFile.h"
#define ASSET_MANAGEMENT_CONFIG_FILENAME "AssetManagementConfig.json"

namespace GameSmith {
	class AssetManagementConfig : public JsonFile {
	public:
		AssetManagementConfig() : JsonFile(), m_AssetDirectory("") {}
		virtual ~AssetManagementConfig() = default;
		inline const std::string& GetAssetDirectory() const { return m_AssetDirectory; }
		void SetAssetDirectory(const std::string& dir);
	protected:
		void ExtractJsonData(const json& newJson) override;
	private:
		std::string m_AssetDirectory;
	};
};
