#pragma once

namespace GameSmithEditor {
	bool PickFolderDialog(std::string path, std::string* outPath);
	std::string PickFileDialog(std::string path, std::string fileType, std::string filePrefix);
	std::string CreateFileDialog(std::string path, std::string fileType, std::string filePrefix);
};


