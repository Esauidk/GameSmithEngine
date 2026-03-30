#include "gepch.h"
#include "ShaderMetadata.h"

#define PROPERTIES_KEY "Properties"

namespace GameSmith {
	void ShaderMetadata::ExtractJsonData(const json& newJson)
	{
		if (newJson.contains(PROPERTIES_KEY)) {
			m_Properties = newJson[PROPERTIES_KEY].get<std::unordered_map<std::string, ShaderDataType>>();
		}
	}

	void ShaderMetadata::RefreshConfig()
	{
		json newJson;
		newJson[PROPERTIES_KEY] = m_Properties;
		UpdateJson(newJson);
	}
};