#include "gepch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace GameSmith {
	std::shared_ptr<spdlog::logger> Log::p_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::p_AppLogger;

	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v(%s)%$");

		p_CoreLogger = spdlog::stdout_color_mt("GAMESMITHENGINE");
		p_CoreLogger->set_level(spdlog::level::trace);

		p_AppLogger = spdlog::stdout_color_mt("APPLICATION");
		p_AppLogger->set_level(spdlog::level::trace);
	}

	std::shared_ptr<spdlog::logger>& Log::GetCoreLogger()
	{
		return p_CoreLogger;
	}

	std::shared_ptr<spdlog::logger>& Log::GetAppLogger()
	{
		return p_AppLogger;
	}
};
