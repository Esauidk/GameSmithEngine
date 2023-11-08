#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace GameSmith {
	// A logger class that allows the engine and clients to send types of logs to the console
	class Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return p_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetAppLogger() { return p_AppLogger; }
	private:
		static std::shared_ptr<spdlog::logger> p_CoreLogger;
		static std::shared_ptr<spdlog::logger> p_AppLogger;
	};

};

// Core Log Macros
#define GE_CORE_INFO(...) GameSmith::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GE_CORE_ERROR(...) GameSmith::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GE_CORE_WARN(...) GameSmith::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GE_CORE_TRACE(...) GameSmith::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GE_CORE_CRITICAL(...) GameSmith::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Log Macros
#define GE_APP_INFO(...) GameSmith::Log::GetAppLogger()->info(__VA_ARGS__)
#define GE_APP_ERROR(...) GameSmith::Log::GetAppLogger()->error(__VA_ARGS__)
#define GE_APP_WARN(...) GameSmith::Log::GetAppLogger()->warn(__VA_ARGS__)
#define GE_APP_TRACE(...) GameSmith::Log::GetAppLogger()->trace(__VA_ARGS__)
#define GE_APP_CRITICAL(...) GameSmith::Log::GetAppLogger()->critical(__VA_ARGS__)


