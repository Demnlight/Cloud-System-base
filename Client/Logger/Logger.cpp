#include "Logger.h"

#include <Windows.h>
#include <iostream>
#include <chrono>
#include <cstdarg>
#include <fstream>
#include <filesystem>

std::mutex C_Logger::_mutex{};

struct Prefix
{
	char color;
	const char* text;
};

Prefix GetLevelPrefix(C_Logger::Level level)
{
	switch (level)
	{
	case C_Logger::Level::Critical:
		return { 0x04, "Critical" };
	case C_Logger::Level::Error:
		return { 0x0C, "Error" };
	case C_Logger::Level::Warning:
		return { 0x06, "Warning" };
	case C_Logger::Level::Info:
		return { 0x02, "Info" };
	case C_Logger::Level::Debug:
		return { 0x0B, "Debug" };
	case C_Logger::Level::Trace:
		return { 0x08, "Trace" };
	default:
		return { 0x00, "" };
	}
}

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
	auto size = static_cast<size_t>(size_s);
	auto buf = std::make_unique<char[]>(size);
	std::snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

void C_Logger::Log(C_Logger::Level logLevel, const char* filepath, int line, const char* fmt, ...)
{
	char buffer[1024];

	va_list args;
	va_start(args, fmt);
	vsprintf_s(buffer, fmt, args);
	va_end(args);

	auto prefix = GetLevelPrefix(logLevel);
	auto filename = std::filesystem::path(filepath).filename().string();

	const std::lock_guard<std::mutex> lock(_mutex);

	auto logLineConsole = string_format("[%s:%d] %s", filename.c_str(), line, buffer);

#if !_DEBUG
	if (NDEBUG == 1 && logLevel == C_Logger::Level::Debug)
		return;
#endif

	std::cout << "[";

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, prefix.color);
	std::cout << prefix.text;
	SetConsoleTextAttribute(hConsole, 15);

	std::cout << "] " << logLineConsole << std::endl;
}