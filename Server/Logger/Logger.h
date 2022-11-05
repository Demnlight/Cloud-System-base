#pragma once

#include <string>
#include <mutex>

#define EXTLOG(level, fmt, ...) C_Logger::Log(level, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define LOG_CRIT(fmt, ...) EXTLOG(C_Logger::Level::Critical, fmt, __VA_ARGS__)
#define LOG_ERROR(fmt, ...) EXTLOG(C_Logger::Level::Error, fmt, __VA_ARGS__)
#define LOG_WARNING(fmt, ...) EXTLOG(C_Logger::Level::Warning, fmt, __VA_ARGS__)
#define LOG_INFO(fmt, ...) EXTLOG(C_Logger::Level::Info, fmt, __VA_ARGS__)
#define LOG_DEBUG(fmt, ...) EXTLOG(C_Logger::Level::Debug, fmt, __VA_ARGS__)
#define LOG_TRACE(fmt, ...) EXTLOG(C_Logger::Level::Trace, fmt, __VA_ARGS__)


class C_Logger
{
public:
	enum class Level
	{
		None,
		Critical,
		Error,
		Warning,
		Info,
		Debug,
		Trace
	};

	static void Log(Level logLevel, const char* filename, int line, const char* fmt, ...);

private:
	static std::mutex _mutex;
};

inline C_Logger* g_Logger = new C_Logger();