#pragma once

#if defined(_WIN32)
#	define AR_PLATFORM_OS_WINDOWS 1
#elif defined(__linux__)
#	define AR_PLATFORM_OS_LINUX 1
#else
#	error "Unsupported platform."
#endif

#if defined(__GNUC__)
#	define AR_PLATFORM_COMPILER_GCC 1
#	define AR_PLATFORM_COMPILER_VERSION_MAJOR __GNUC__
#	define AR_PLATFORM_COMPILER_VERSION_MINOR __GNUC_MINOR__
#	define AR_PLATFORM_COMPILER_VERSION_PATCHLEVEL __GNUC_PATCHLEVEL__

#	define AR_INT8_TYPE __INT8_TYPE__
#	define AR_INT16_TYPE __INT16_TYPE__
#	define AR_INT32_TYPE __INT32_TYPE__
#	define AR_INT64_TYPE __INT64_TYPE__

#	define AR_UINT8_TYPE __UINT8_TYPE__
#	define AR_UINT16_TYPE __UINT16_TYPE__
#	define AR_UINT32_TYPE __UINT32_TYPE__
#	define AR_UINT64_TYPE __UINT64_TYPE__

#	define AR_FLOAT32_TYPE float
#	define AR_FLOAT64_TYPE double

#	define AR_CHAR8_TYPE char
#	define AR_CHAR16_TYPE wchar_t
#elif defined(__clang__)

#elif defined(_MSC_VER)

#endif
namespace Arcane { }
