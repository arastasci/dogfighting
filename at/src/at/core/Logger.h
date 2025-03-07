#pragma once

#include <atpch.h>

#include "at/core/Core.h"

#include <spdlog/spdlog.h>

namespace at {

	class AT_API Logger
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}


// Core log macros
#define AT_CORE_TRACE(...)    ::at::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define AT_CORE_INFO(...)     ::at::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define AT_CORE_WARN(...)     ::at::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define AT_CORE_ERROR(...)    ::at::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define AT_CORE_CRITICAL(...) ::at::Logger::GetCoreLogger()->critical(__VA_ARGS__)
									   
// Client log macros				   
#define AT_TRACE(...)         ::at::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define AT_INFO(...)          ::at::Logger::GetClientLogger()->info(__VA_ARGS__)
#define AT_WARN(...)          ::at::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define AT_ERROR(...)         ::at::Logger::GetClientLogger()->error(__VA_ARGS__)
#define AT_CRITICAL(...)      ::at::Logger::GetClientLogger()->critical(__VA_ARGS__)