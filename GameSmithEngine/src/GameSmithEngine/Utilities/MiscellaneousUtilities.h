#pragma once
#include <string>
namespace GameSmith {
	// A collection of utilities that do not fit into a specific category but are used in multiple places
	std::wstring ToWideString(const std::string& toConvert);

	std::string ToNarrowString(const std::wstring& toConvert);
}