#include "Log.hpp"

#include <cstdarg>
#include <cstdio>

namespace Arcane {

	Logger::Logger(const std::string &name) : mName(name), mLevel(LogLevel::All) { }

	Logger::~Logger() { }

	void Logger::SetLevel(LogLevel level) {
		mLevel = level;
	}

	void Logger::Log(LogLevel level, const char *message, ...) {
		if (level > mLevel) return;

		va_list args;
		va_start(args, message);
		
		char buffer[2056] = {0};
		vsnprintf(buffer, sizeof(buffer), message, args);
		switch (level) {
			case LogLevel::Fatal:   printf("\033[101m\033[97m[%s] FATAL: %s\033[0m\n", mName.c_str(), buffer); break;
			case LogLevel::Error:   printf("\033[31m[%s] ERROR: %s\033[0m\n", mName.c_str(), buffer); break;
			case LogLevel::Warning: printf("\033[33m[%s] WARNING: %s\033[0m\n", mName.c_str(), buffer); break;
			case LogLevel::Debug:   printf("\033[32m[%s] DEBUG: %s\033[0m\n", mName.c_str(), buffer); break;
			case LogLevel::Info:    printf("\033[34m[%s] INFO: %s\033[0m\n", mName.c_str(), buffer); break;
			case LogLevel::Trace:   printf("\033[35m[%s] TRACE: %s\033[0m\n", mName.c_str(), buffer); break;
		}

		va_end(args);
	}

	Logger &GetEngineLogger() {
		static Logger logger("Arcane");
		return logger;
	}

}