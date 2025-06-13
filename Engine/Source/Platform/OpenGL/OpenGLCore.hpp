#pragma once

#include <Arcane/Core.hpp>
#include "OpenGL.hpp"

#ifdef TRACY_ENABLE
#	include "tracy/TracyOpenGL.hpp"
#	define AR_PROFILE_GPU_CONTEXT(name) TracyGpuContext; TracyGpuContextName(name, sizeof(name))
#	define AR_PROFILE_FUNCTION_GPU() TracyGpuZone(__PRETTY_FUNCTION__)
#	define AR_PROFILE_FUNCTION_GPU_CPU() TracyGpuZone(__PRETTY_FUNCTION__) ZoneScopedN(__PRETTY_FUNCTION__)
#	define AR_PROFILE_SCOPE_GPU(name) TracyGpuZone(name)
#	define AR_PROFILE_SCOPE_GPU_CPU(name) TracyGpuZone(name) ZoneScopedN(name)
#	define AR_PROFILE_GPU_COLLECT() TracyGpuCollect
#endif

#ifdef _DEBUG
#	define AR_OPENGL_ASSERT(x, ...) { if (!(x)) { ::Arcane::GetOpenGLLogger().Log(LogLevel::Fatal, __VA_ARGS__); __debugbreak(); } }
#	define AR_OPENGL_TRACE(...) ::Arcane::GetOpenGLLogger().Log(Arcane::LogLevel::Trace, __VA_ARGS__)
#	define AR_OPENGL_INFO(...) ::Arcane::GetOpenGLLogger().Log(Arcane::LogLevel::Info, __VA_ARGS__)
#	define AR_OPENGL_DEBUG(...) ::Arcane::GetOpenGLLogger().Log(Arcane::LogLevel::Debug, __VA_ARGS__)
#	define AR_OPENGL_WARNING(...) ::Arcane::GetOpenGLLogger().Log(Arcane::LogLevel::Warning, __VA_ARGS__)
#	define AR_OPENGL_ERROR(...) ::Arcane::GetOpenGLLogger().Log(Arcane::LogLevel::Error, __VA_ARGS__)
#	define AR_OPENGL_FATAL(...) ::Arcane::GetOpenGLLogger().Log(Arcane::LogLevel::Fatal, __VA_ARGS__)
#else
#	define AR_OPENGL_ASSERT(x, ...)
#	define AR_OPENGL_TRACE(...)
#	define AR_OPENGL_INFO(...)
#	define AR_OPENGL_DEBUG(...)
#	define AR_OPENGL_WARNING(...)
#	define AR_OPENGL_ERROR(...)
#	define AR_OPENGL_FATAL(...)
#endif

namespace Arcane {

	Logger &GetOpenGLLogger();

}