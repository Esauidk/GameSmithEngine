#include "MainException.h"
#include <sstream>

MainException::MainException(int line, const char* file) noexcept : line(line), file(file) {}

const char* MainException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* MainException::GetType() const noexcept {
	return "Main Exception";
}

int MainException::GetLine() const noexcept {
	return line;
}

const std::string& MainException::GetFile() const noexcept {
	return file;
}

std::string MainException::GetOriginString() const noexcept {
	std::ostringstream oss;
	oss << "[File] " << file << std::endl << "[Line] " << line;
	return oss.str();
}