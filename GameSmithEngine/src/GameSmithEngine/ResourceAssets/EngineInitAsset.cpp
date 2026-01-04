#include "gepch.h"
#include "EngineInitAsset.h"

#include "GameSmithEngine/ResourceManagement/ResourceAssetHelper.h"

#include "nlohmann/json.hpp"

#define ENGINE_OVERRIDES "EngineOverrides"
#define ASSET_DIRECTORY_KEY "OverideAssetDirectory"
#define CONTENT_DIRECTORY_KEY "OverideContentDirectory"

using json = nlohmann::json;

namespace GameSmith {
	Ref<char> EngineInitAsset::Serialize()
	{
		json jsonFile = {
			{ENGINE_OVERRIDES, {
					{ASSET_DIRECTORY_KEY, m_OverrideAssetDirectory},
					{CONTENT_DIRECTORY_KEY, m_OverrideContentDirectory}
			}}
		};

		std::string jsonString = jsonFile.dump(4);

		BinaryStreamWriter writer(jsonString.length() + 1); // +1 for null terminator
		writer.WriteString(jsonString);

		return writer.GetBuffer();
	}

	void EngineInitAsset::Serialize(char* byteStream, unsigned int availableBytes)
	{
	}

	unsigned int EngineInitAsset::RequiredSpace() const
	{
		json jsonFile = {
			{ENGINE_OVERRIDES, {
					{ASSET_DIRECTORY_KEY, m_OverrideAssetDirectory},
					{CONTENT_DIRECTORY_KEY, m_OverrideContentDirectory}
			}}
		};

		std::string jsonString = jsonFile.dump(4);

		return jsonString.length() + 1; // +1 for null terminator
	}

	void EngineInitAsset::Deserialize(char* inData, unsigned int size)
	{
	}

};

