#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace ProjectGE {
	class GE_API Log
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
#define GE_CORE_INFO(...) ProjectGE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GE_CORE_ERROR(...) ProjectGE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GE_CORE_WARN(...) ProjectGE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GE_CORE_TRACE(...) ProjectGE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GE_CORE_FATAL(...) ProjectGE::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client Log Macros
#define GE_APP_INFO(...) ProjectGE::Log::GetAppLogger()->info(__VA_ARGS__)
#define GE_APP_ERROR(...) ProjectGE::Log::GetAppLogger()->error(__VA_ARGS__)
#define GE_APP_WARN(...) ProjectGE::Log::GetAppLogger()->warn(__VA_ARGS__)
#define GE_APP_TRACE(...) ProjectGE::Log::GetAppLogger()->trace(__VA_ARGS__)
#define GE_APP_FATAL(...) ProjectGE::Log::GetAppLogger()->fatal(__VA_ARGS__)


