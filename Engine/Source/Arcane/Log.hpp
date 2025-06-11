#pragma once

#include <string>

#ifdef _DEBUG
#	define AR_ASSERT(x, ...) { if (!(x)) { ::Arcane::GetEngineLogger().Log(LogLevel::Fatal, __VA_ARGS__); __debugbreak(); } }
#	define AR_ENGINE_LOG(level, message, ...) { ::Arcane::GetEngineLogger().Log(level, message, __VA_ARGS__); }
#	define AR_ENGINE_TRACE(message, ...) { ::Arcane::GetEngineLogger().Log(LogLevel::Trace, message, __VA_ARGS__); }
#	define AR_ENGINE_INFO(message, ...) { ::Arcane::GetEngineLogger().Log(LogLevel::Info, message, __VA_ARGS__); }
#	define AR_ENGINE_DEBUG(message, ...) { ::Arcane::GetEngineLogger().Log(LogLevel::Debug, message, __VA_ARGS__); }
#	define AR_ENGINE_WARNING(message, ...) { ::Arcane::GetEngineLogger().Log(LogLevel::Warning, message, __VA_ARGS__); }
#	define AR_ENGINE_ERROR(message, ...) { ::Arcane::GetEngineLogger().Log(LogLevel::Error, message, __VA_ARGS__); }
#	define AR_ENGINE_FATAL(message, ...) { ::Arcane::GetEngineLogger().Log(LogLevel::Fatal, message, __VA_ARGS__); }
#else
#	define AR_ENGINE_LOG(level, message, ...)
#	define AR_ENGINE_TRACE(message, ...)
#	define AR_ENGINE_INFO(message, ...)
#	define AR_ENGINE_DEBUG(message, ...)
#	define AR_ENGINE_WARNING(message, ...)
#	define AR_ENGINE_ERROR(message, ...)
#	define AR_ENGINE_FATAL(message, ...)
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
		Logger(const std::string &name);
		~Logger();

		void SetLevel(LogLevel level);
		void Log(LogLevel level, const char *message, ...);

	private:
		std::string mName;
		LogLevel mLevel;
	};

	Logger &GetEngineLogger();

}