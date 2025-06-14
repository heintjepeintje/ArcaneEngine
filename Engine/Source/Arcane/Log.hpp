#pragma once

#include <string>

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