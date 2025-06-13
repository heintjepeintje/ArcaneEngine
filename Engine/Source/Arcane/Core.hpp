#pragma once

#include <memory>
#include <string>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <filesystem>

#include "Ref.hpp"
#include "Log.hpp"

#define AR_VERSION_MAJOR 0
#define AR_VERSION_MINOR 1
#define AR_VERSION_PATCH 0

#ifdef TRACY_ENABLE
#	define TRACY_ONLY_LOCALHOST 1
#	include <tracy/Tracy.hpp>
#	define AR_PROFILE_FUNCTION() ZoneScopedN(__PRETTY_FUNCTION__)
#	define AR_PROFILE_SCOPE(name) ZoneScopedN(name)
#	define AR_PROFILE_FRAME_START() FrameMarkStart(nullptr)
#	define AR_PROFILE_FRAME_END() FrameMarkEnd(nullptr)
#else 
#	define AR_PROFILE_FUNCTION()
#	define AR_PROFILE_SCOPE(name)
#	define AR_PROFILE_FRAME()
#endif

#define AR_BIT(x) (1 << x)

#define AR_PTR_ADD(ptr, offset) ((void*)((uintptr_t)ptr + offset))

#define AR_STRCAT(a, b) a##b