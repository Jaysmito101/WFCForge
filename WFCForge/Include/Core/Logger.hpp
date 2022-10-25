#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdarg>
#include <atomic>
#include <thread>

namespace WFCForge
{
	enum LogLevel
	{
		LogLevel_Info = 0,
		LogLevel_Warning,
		LogLevel_Error,
		LogLeve_Count
	};

	class Logger
	{
	public:

		static void Initalize(std::string loggerPath);
		static void Shutdown();
		static void Flush();
		static void Log(LogLevel level, const char* format...);


	private:
		static std::string logBuffer;
		static std::string logPath;
		static std::atomic<bool> flushing;
	};
}

#ifndef WFC_DISABLE_LOGGING

#define WFC_LOGGER_INITIALIZE(path) ::WFCForge::Logger::Initalize(path);
#define WFC_LOGGER_SHUTDOWN()		 ::WFCForge::Logger::Shutdown();
#define WFC_LOGGER_FLUSH()			 ::WFCForge::Logger::Flush();

#define WFC_LOG(...)			WFC_LOG_INFO(__VA_ARGS__)
#define WFC_LOG_INFO(...)		::WFCForge::Logger::Log(WFCForge::LogLevel::LogLevel_Info, __VA_ARGS__);
#define WFC_LOG_WARN(...)		::WFCForge::Logger::Log(WFCForge::LogLevel::LogLevel_Warning, __VA_ARGS__);
#define WFC_LOG_ERROR(...)		::WFCForge::Logger::Log(WFCForge::LogLevel::LogLevel_Error, __VA_ARGS__);


#else

#define WFC_LOGGER_INITIALIZE(path)
#define WFC_LOGGER_SHUTDOWN()
#define WFC_LOGGER_FLUSH()

#define WFC_LOG_INFO(...)
#define WFC_LOG_WARN(...)
#define WFC_LOG_ERROR(...)

#endif