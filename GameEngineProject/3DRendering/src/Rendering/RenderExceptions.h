#pragma once
#include "../../MainException.h"
#include <vector>
#include <wrl.h>

namespace Render {
	class RenderExceptions {
	public:
		// Normal Exception
		class Exception : public MainException {
			using MainException::MainException;
		public:
			static std::string TranslateErrorCode(HRESULT hr) noexcept;
		};
		// Exceptions relating to HRESULTs
		class HrException : public Exception {
		public:
			HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsg = {}) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorDescription() const noexcept;
			std::string GetErrorInfo() const noexcept;
		private:
			HRESULT hr;
			std::string info;
		};
	};

};
