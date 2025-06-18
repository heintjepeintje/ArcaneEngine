#pragma once

#include <Arcane/Core.hpp>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#ifdef _WIN32
#	include <windef.h>
#	include <winbase.h>
#	include <winuser.h>
#	include <vulkan/vulkan_win32.h>
#endif

#define AR_VULKAN_GET_FUNCTION(name) PFN_##name name = (PFN_##name)vkGetInstanceProcAddr(mInstance, #name)

#ifdef _DEBUG
#	define AR_VULKAN_ASSERT(x, ...) { if (!(x)) { ::Arcane::GetVulkanLogger().Log(LogLevel::Fatal, __VA_ARGS__); __debugbreak(); } }
#	define AR_VULKAN_CHECK_RESULT(result) AR_VULKAN_ASSERT(result == VK_SUCCESS, "{}", (uint32_t)result)
#	define AR_VULKAN_TRACE(...) ::Arcane::GetVulkanLogger().Log(Arcane::LogLevel::Trace, __VA_ARGS__)
#	define AR_VULKAN_INFO(...) ::Arcane::GetVulkanLogger().Log(Arcane::LogLevel::Info, __VA_ARGS__)
#	define AR_VULKAN_DEBUG(...) ::Arcane::GetVulkanLogger().Log(Arcane::LogLevel::Debug, __VA_ARGS__)
#	define AR_VULKAN_WARNING(...) ::Arcane::GetVulkanLogger().Log(Arcane::LogLevel::Warning, __VA_ARGS__)
#	define AR_VULKAN_ERROR(...) ::Arcane::GetVulkanLogger().Log(Arcane::LogLevel::Error, __VA_ARGS__)
#	define AR_VULKAN_FATAL(...) ::Arcane::GetVulkanLogger().Log(Arcane::LogLevel::Fatal, __VA_ARGS__)
#else
#	define AR_VULKAN_ASSERT(x, ...)
#	define AR_VULKAN_CHECK_RESULT(result)
#	define AR_VULKAN_TRACE(...)
#	define AR_VULKAN_INFO(...)
#	define AR_VULKAN_DEBUG(...)
#	define AR_VULKAN_WARNING(...)
#	define AR_VULKAN_ERROR(...)
#	define AR_VULKAN_FATAL(...)
#endif


namespace Arcane {

	Logger &GetVulkanLogger();

}