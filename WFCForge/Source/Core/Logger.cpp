#include "Core/Logger.hpp"
#include "Core/Utils.hpp"

namespace WFCForge
{
	std::string Logger::logBuffer = "";
	std::string Logger::logPath = "Log.txt"; 
	std::atomic<bool> Logger::flushing = false;

	static const char* logLevelName[] = 
	{
		"INFO",
		"WARNING",
		"ERROR"
	};

	void Logger::Initalize(std::string path)
	{
		logPath = path;
		logBuffer.reserve(4096);
		Logger::Log(LogLevel::LogLevel_Info, "-------------- Logger Session Initialized --------------");
	}

	void Logger::Shutdown()
	{
		Logger::Log(LogLevel::LogLevel_Info, "-------------- Logger Session Shutdown -----------------");
		Logger::Flush();
		while (flushing);
	}

	void Logger::Log(LogLevel level, const char* format...)
	{
		static char buffer[4096];
		static char buffer2[4096];
		buffer[0] = '\0';
		buffer2[0] = '\0';
		va_list vargs;
		va_start(vargs, format);
		vsprintf(buffer, format, vargs);
		va_end(vargs);
		sprintf(buffer2, "%s: %s\n", logLevelName[level], buffer);
		printf(buffer2);
		logBuffer += std::string(buffer2);

		if (logBuffer.size() > 4096)
			Logger::Flush();
	}

	void Logger::Flush()
	{
		while (flushing);
		flushing = true;
		std::string flushBuffer = logBuffer;
		logBuffer = "";
		logBuffer.reserve(4096);
		std::thread flusher([flushBuffer]()->void {
			Utils::AppendFile(logPath, flushBuffer);
			flushing = false;
			});
		flusher.detach();
	}
	
}