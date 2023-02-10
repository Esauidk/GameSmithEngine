#include "RenderExceptions.h" 
#include <sstream>


using namespace Render;

RenderExceptions::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept : Exception(line, file), hr(hr) {
	for (const auto& m : infoMsgs) {
		info += m;
		info.push_back('\n');
	}

	if (!info.empty()) {
		info.pop_back();
	}
}

const char* RenderExceptions::HrException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;
	if (!info.empty()) {
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::string RenderExceptions::HrException::GetErrorInfo() const noexcept {
	return info;
}
const char* RenderExceptions::HrException::GetType() const noexcept {
	return "Graphics HResult Exception";
}

HRESULT RenderExceptions::HrException::GetErrorCode() const noexcept { return hr; }

std::string RenderExceptions::HrException::GetErrorDescription() const noexcept {
	return TranslateErrorCode(hr);
}

std::string RenderExceptions::Exception::TranslateErrorCode(HRESULT hr) noexcept {
	char* pMsgBuf = nullptr;
	return std::system_category().message(hr);
}