#pragma once

#include "OpenGLCore.hpp"
#ifdef TRACY_ENABLE
#	include "tracy/TracyOpenGL.hpp"
#	define AR_PROFILE_GPU_CONTEXT(name) TracyGpuContext; TracyGpuContextName(name, sizeof(name))
#	define AR_PROFILE_FUNCTION_GPU() TracyGpuZone(__PRETTY_FUNCTION__)
#	define AR_PROFILE_FUNCTION_GPU_CPU() TracyGpuZone(__PRETTY_FUNCTION__) ZoneScopedN(__PRETTY_FUNCTION__)
#	define AR_PROFILE_SCOPE_GPU(name) TracyGpuZone(name)
#	define AR_PROFILE_SCOPE_GPU_CPU(name) TracyGpuZone(name) ZoneScopedN(name)
#	define AR_PROFILE_GPU_COLLECT() TracyGpuCollect
#endif