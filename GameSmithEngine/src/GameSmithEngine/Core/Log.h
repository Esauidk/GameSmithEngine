#pragma once

#include "Core.h"

#pragma warning(disable: 4996)

// To avoid SPDLog from defining this
#define SPDLOG_ACTIVE_LEVEL 0

#include "spdlog/spdlog.h"
// To define using other SPDLog macros
#undef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_CRITICAL
#include "spdlog/fmt/ostr.h"

namespace GameSmith {
	// A logger class that allows the engine and clients to send types of logs to the console
	class GE_API Log
	{
	public:
		static void Init();
		static std::shared_ptr<spdlog::logger>& GetCoreLogger();
		static std::shared_ptr<spdlog::logger>& GetAppLogger();
	private:
		static std::shared_ptr<spdlog::logger> p_CoreLogger;
		static std::shared_ptr<spdlog::logger> p_AppLogger;
	};

};

#ifdef GE_DEBUG
// Core Log Macros
#define GE_CORE_INFO(...) SPDLOG_LOGGER_INFO(GameSmith::Log::GetCoreLogger(), __VA_ARGS__)
#define GE_CORE_ERROR(...) SPDLOG_LOGGER_ERROR(GameSmith::Log::GetCoreLogger(), __VA_ARGS__)
#define GE_CORE_WARN(...) SPDLOG_LOGGER_WARN(GameSmith::Log::GetCoreLogger(), __VA_ARGS__)
#define GE_CORE_TRACE(...) SPDLOG_LOGGER_TRACE(GameSmith::Log::GetCoreLogger(), __VA_ARGS__)
#define GE_CORE_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(GameSmith::Log::GetCoreLogger(), __VA_ARGS__)

// Client Log Macros
#define GE_APP_INFO(...) SPDLOG_LOGGER_INFO(GameSmith::Log::GetAppLogger(), __VA_ARGS__)
#define GE_APP_ERROR(...) SPDLOG_LOGGER_ERROR(GameSmith::Log::GetAppLogger(), __VA_ARGS__)
#define GE_APP_WARN(...) SPDLOG_LOGGER_WARN(GameSmith::Log::GetAppLogger(), __VA_ARGS__)
#define GE_APP_TRACE(...) SPDLOG_LOGGER_TRACE(GameSmith::Log::GetAppLogger(), __VA_ARGS__)
#define GE_APP_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(GameSmith::Log::GetAppLogger(), __VA_ARGS__)
#else
// Core Log Macros
#define GE_CORE_INFO(...)
#define GE_CORE_ERROR(...)
#define GE_CORE_WARN(...)
#define GE_CORE_TRACE(...)
#define GE_CORE_CRITICAL(...)

// Client Log Macros
#define GE_APP_INFO(...)
#define GE_APP_ERROR(...)
#define GE_APP_WARN(...)
#define GE_APP_TRACE(...)
#define GE_APP_CRITICAL(...)
#endif



