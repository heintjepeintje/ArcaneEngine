#pragma once

#include <string>
#include <format>
#include <iostream>

#ifdef _DEBUG
#	define AR_ASSERT(x, ...) { if (!(x)) { ::Arcane::GetEngineLogger().Log(LogLevel::Fatal, __VA_ARGS__); __debugbreak(); } }
#	define AR_ENGINE_LOG(level, ...) { ::Arcane::GetEngineLogger().Log(level, message, __VA_ARGS__); }
#	define AR_ENGINE_TRACE(...) { ::Arcane::GetEngineLogger().Log(LogLevel::Trace, __VA_ARGS__); }
#	define AR_ENGINE_INFO(...) { ::Arcane::GetEngineLogger().Log(LogLevel::Info, __VA_ARGS__); }
#	define AR_ENGINE_DEBUG(...) { ::Arcane::GetEngineLogger().Log(LogLevel::Debug, __VA_ARGS__); }
#	define AR_ENGINE_WARNING(...) { ::Arcane::GetEngineLogger().Log(LogLevel::Warning, __VA_ARGS__); }
#	define AR_ENGINE_ERROR(...) { ::Arcane::GetEngineLogger().Log(LogLevel::Error, __VA_ARGS__); }
#	define AR_ENGINE_FATAL(...) { ::Arcane::GetEngineLogger().Log(LogLevel::Fatal, __VA_ARGS__); }
#else
#	define AR_ENGINE_LOG(level, ...)
#	define AR_ENGINE_TRACE(...)
#	define AR_ENGINE_INFO(...)
#	define AR_ENGINE_DEBUG(...)
#	define AR_ENGINE_WARNING(...)
#	define AR_ENGINE_ERROR(...)
#	define AR_ENGINE_FATAL(...)
#	define AR_ASSERT(x, ...)
#endif

namespace Arcane {

	enum class LogLevel {
		None = 0,
		Fatal, Error, Warning,
		Debug, Info, Trace,
		All
	};

	class Logger {
	public:
		Logger(const std::string &name) : mName(name), mLevel(LogLevel::All) { }
		~Logger() { }

		void SetLevel(LogLevel level) { mLevel = level; }

		template<typename ..._Args>
		inline void Log(LogLevel level, const std::format_string<_Args...> message, _Args &&...args) {
			if (level < mLevel) return;
			const std::string fmt = std::format(message, std::forward<_Args>(args)...);
			switch (level) {
				case LogLevel::Fatal:   std::cout << "\033[101m\033[97m[" << mName << "] FATAL: " << fmt << "\033[0m\n"; break;
				case LogLevel::Error:   std::cout << "\033[31m[" << mName << "] ERROR: " << fmt << "\033[0m\n"; break;
				case LogLevel::Warning: std::cout << "\033[33m[" << mName << "] WARNING: " << fmt << "\033[0m\n"; break;
				case LogLevel::Debug:   std::cout << "\033[32m[" << mName << "] DEBUG: " << fmt << "\033[0m\n"; break;
				case LogLevel::Info:    std::cout << "\033[34m[" << mName << "] INFO: " << fmt << "\033[0m\n"; break;
				case LogLevel::Trace:   std::cout << "\033[35m[" << mName << "] TRACE: " << fmt << "\033[0m\n"; break;
			}
		}

	private:
		std::string mName;
		LogLevel mLevel;
	};

	Logger &GetEngineLogger();

}