#include "gepch.h"
#include "MiscellaneousUtilities.h"
#include <stringapiset.h>
std::wstring GameSmith::ToWideString(const std::string& toConvert)
{
#ifdef GE_PLATFORM_WINDOWS
	int expectedSize = MultiByteToWideChar(CP_UTF8, 0, toConvert.c_str(), toConvert.size(), nullptr, 0);
	std::wstring result(expectedSize, 0);
	MultiByteToWideChar(CP_UTF8, 0, toConvert.c_str(), toConvert.size(), result.data(), expectedSize);

	return result;
#endif // GE_PLATFORM_WINDOW

	return std::wstring();
}

std::string GameSmith::ToNarrowString(const std::wstring& toConvert)
{
#ifdef GE_PLATFORM_WINDOWS
	int expectedSize = WideCharToMultiByte(CP_UTF8, 0, toConvert.c_str(), toConvert.size(), nullptr, 0, nullptr, nullptr);
	std::string result(expectedSize, 0);
	WideCharToMultiByte(CP_UTF8, 0, toConvert.c_str(), toConvert.size(), result.data(), expectedSize, nullptr, nullptr);
	return result;
#endif // GE_PLATFORM_WINDOWS

	return std::string();
}
