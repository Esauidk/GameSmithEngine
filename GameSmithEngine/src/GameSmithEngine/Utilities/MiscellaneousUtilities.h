#pragma once
#include <string>
namespace GameSmith {
	// A collection of utilities that do not fit into a specific category but are used in multiple places
	std::wstring ToWideString(const std::string& toConvert);

	std::string ToNarrowString(const std::wstring& toConvert);

	inline void combine_hash(std::size_t& seed, std::size_t value) {
		seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
}