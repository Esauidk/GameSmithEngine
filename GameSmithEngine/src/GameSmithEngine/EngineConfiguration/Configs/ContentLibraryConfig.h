#pragma once

#include "GameSmithEngine/SerializeableFiles/GenericFileTypes/JsonFile.h"
#define CONTENT_LIBRARY_CONFIG_FILENAME "ContentLibraryConfig.json"

namespace GameSmith {
	class ContentLibraryConfig : public JsonFile {
	public:
		ContentLibraryConfig() : JsonFile(), m_ContentLibraryDirectory("") {}
		virtual ~ContentLibraryConfig() = default;
		inline const std::string& GetContentLibraryDirectory() const { return m_ContentLibraryDirectory; }
		void SetContentLibraryDirectory(const std::string& dir);
	protected:
		void ExtractJsonData(const json& newJson) override;
	private:
		std::string m_ContentLibraryDirectory;
	};
};
