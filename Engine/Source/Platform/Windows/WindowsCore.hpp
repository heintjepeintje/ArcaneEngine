#pragma once

#ifdef _WIN32

#include <Arcane/Core.hpp>

#include <windef.h>
#include <winbase.h>
#include <winuser.h>
#include <wingdi.h>
#include <ws2tcpip.h>
#include <winsock.h>
#include <winsock2.h>
#include <wininet.h>

#ifdef _DEBUG
#	define AR_WINDOWS_ASSERT(x, ...) { if (!(x)) { ::Arcane::GetWindowsLogger().Log(LogLevel::Fatal, __VA_ARGS__); __debugbreak(); } }
#	define AR_WINDOWS_TRACE(...) ::Arcane::GetWindowsLogger().Log(Arcane::LogLevel::Trace, __VA_ARGS__)
#	define AR_WINDOWS_INFO(...) ::Arcane::GetWindowsLogger().Log(Arcane::LogLevel::Info, __VA_ARGS__)
#	define AR_WINDOWS_DEBUG(...) ::Arcane::GetWindowsLogger().Log(Arcane::LogLevel::Debug, __VA_ARGS__)
#	define AR_WINDOWS_WARNING(...) ::Arcane::GetWindowsLogger().Log(Arcane::LogLevel::Warning, __VA_ARGS__)
#	define AR_WINDOWS_ERROR(...) ::Arcane::GetWindowsLogger().Log(Arcane::LogLevel::Error, __VA_ARGS__)
#	define AR_WINDOWS_FATAL(...) ::Arcane::GetWindowsLogger().Log(Arcane::LogLevel::Fatal, __VA_ARGS__)
#else
#	define AR_WINDOWS_ASSERT(x, ...)
#	define AR_WINDOWS_TRACE(...)
#	define AR_WINDOWS_INFO(...)
#	define AR_WINDOWS_DEBUG(...)
#	define AR_WINDOWS_WARNING(...)
#	define AR_WINDOWS_ERROR(...)
#	define AR_WINDOWS_FATAL(...)
#endif

namespace Arcane {

	Logger &GetWindowsLogger();

} 

#endif // _WIN32