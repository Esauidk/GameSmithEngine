#pragma once

namespace GameSmithEditor {
	struct FileSearchCriteria {
		std::string filePath;
		std::string fileType;
		std::string filePrefix = "*";
	};

	bool PickFolderDialog(const std::string& path, std::string* outPath);
	bool PickFileDialog(const FileSearchCriteria& criteria, std::string* outPath);
	bool CreateFileDialog(const FileSearchCriteria& criteria, std::string* outPath);
};


