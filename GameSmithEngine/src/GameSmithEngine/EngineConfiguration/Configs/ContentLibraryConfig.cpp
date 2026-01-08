#include "gepch.h"
#include "ContentLibraryConfig.h"

#define CONTENT_LIBRARY_DIRECTORY_KEY "ContentLibraryDirectory"
namespace GameSmith {
	void GameSmith::ContentLibraryConfig::SetContentLibraryDirectory(const std::string& dir)
	{
		m_ContentLibraryDirectory = dir;
		json newJson;
		newJson[CONTENT_LIBRARY_DIRECTORY_KEY] = m_ContentLibraryDirectory;
		UpdateJson(newJson);
	}

	void ContentLibraryConfig::ExtractJsonData(const json& newJson)
	{
		if (newJson.contains(CONTENT_LIBRARY_DIRECTORY_KEY)) {
			m_ContentLibraryDirectory = newJson[CONTENT_LIBRARY_DIRECTORY_KEY].get<std::string>();
		}
	}
};