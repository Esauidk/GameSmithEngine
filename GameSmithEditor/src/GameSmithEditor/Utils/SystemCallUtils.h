#pragma once

namespace GameSmithEditor {
	std::string PickFolderDialog(std::string path);
	std::string PickFileDialog(std::string path, std::string fileType, std::string filePrefix);
	std::string CreateFileDialog(std::string path, std::string fileType, std::string filePrefix);
};


