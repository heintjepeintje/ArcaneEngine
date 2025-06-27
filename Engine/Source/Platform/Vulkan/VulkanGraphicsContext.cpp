#include "VulkanGraphicsContext.hpp"

#ifdef _WIN32
#	include <Platform/Windows/Win32Window.hpp>
#endif 

#include <vector>

namespace Arcane {

	VkBool32 DebugMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
		switch (messageSeverity) {
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				AR_VULKAN_TRACE("{}", pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				AR_VULKAN_INFO("{}", pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				AR_VULKAN_WARNING("{}", pCallbackData->pMessage);
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				AR_VULKAN_ERROR("{}", pCallbackData->pMessage);
				break;
		}
		return VK_FALSE;
	}

	VulkanGraphicsContext::VulkanGraphicsContext(const Ref<NativeWindow> &window) : mWindow(window) {
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pNext = nullptr;
		appInfo.pApplicationName = "Arcane Engine";
		appInfo.applicationVersion = VK_MAKE_VERSION(AR_VERSION_MAJOR, AR_VERSION_MINOR, AR_VERSION_PATCH);
		appInfo.pEngineName = "Arcane Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(AR_VERSION_MAJOR, AR_VERSION_MINOR, AR_VERSION_PATCH);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		std::vector<const char *> requiredLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		uint32_t availableLayerCount = 0;
		VkLayerProperties *availableLayers;

		AR_VULKAN_CHECK_RESULT(vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr));
		availableLayers = new VkLayerProperties[availableLayerCount];
		AR_VULKAN_CHECK_RESULT(vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers));

		for (uint32_t i = 0; i < requiredLayers.size(); i++) {
			bool found = false;
			for (uint32_t j = 0; j < availableLayerCount; j++) {
				if (strcmp(requiredLayers[i], availableLayers[j].layerName) == 0) {
					found = true;
					break;
				}
			}
			AR_VULKAN_ASSERT(found, "Could not find required vulkan layer: {}\n", requiredLayers[i]);
		}

		std::vector<const char *> requiredExtensions = {
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
			VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#endif
		};

		uint32_t availableExtensionCount = 0;
		VkExtensionProperties *availableExtensions;

		AR_VULKAN_CHECK_RESULT(vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr));
		availableExtensions = new VkExtensionProperties[availableExtensionCount];
		AR_VULKAN_CHECK_RESULT(vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions));

		for (uint32_t i = 0; i < requiredExtensions.size(); i++) {
			bool found = false;
			for (uint32_t j = 0; j < availableExtensionCount; j++) {
				if (strcmp(requiredExtensions[i], availableExtensions[j].extensionName) == 0) {
					found = true;
					break;
				}
			}
			AR_VULKAN_ASSERT(found, "Could not find required vulkan extension: {}\n", requiredExtensions[i]);
		}

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;
		createInfo.enabledExtensionCount = requiredExtensions.size();
		createInfo.ppEnabledExtensionNames = requiredExtensions.data();
		createInfo.enabledLayerCount = requiredLayers.size();
		createInfo.ppEnabledLayerNames = requiredLayers.data();

		AR_VULKAN_CHECK_RESULT(vkCreateInstance(&createInfo, nullptr, &mInstance));

		delete[] availableLayers;
		delete[] availableExtensions;

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.pNext = nullptr;
		debugCreateInfo.flags = 0;
		debugCreateInfo.messageSeverity = 0
			// | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			// | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
		;
		debugCreateInfo.messageType = 0
			// | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT
			// | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			// | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
		;
		debugCreateInfo.pfnUserCallback = DebugMessageCallback;
		debugCreateInfo.pUserData = nullptr;

		AR_VULKAN_GET_FUNCTION(vkCreateDebugUtilsMessengerEXT);
		AR_VULKAN_CHECK_RESULT(vkCreateDebugUtilsMessengerEXT(mInstance, &debugCreateInfo, nullptr, &mDebugMessenger));

#ifdef _WIN32
		Ref<Win32Window> win32Window = CastRef<Win32Window>(window);

		VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
		surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceCreateInfo.pNext = nullptr;
		surfaceCreateInfo.flags = 0;
		surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
		surfaceCreateInfo.hwnd = win32Window->GetHWND();

		AR_VULKAN_CHECK_RESULT(vkCreateWin32SurfaceKHR(mInstance, &surfaceCreateInfo, nullptr, &mSurface));
#endif

		uint32_t physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, nullptr);
		
		VkPhysicalDevice *physicalDevices = new VkPhysicalDevice[physicalDeviceCount];
		vkEnumeratePhysicalDevices(mInstance, &physicalDeviceCount, physicalDevices);

		// TODO: create "score" system for picking the most powerful device
		std::vector<uint32_t> scores(physicalDeviceCount, 0);
		uint32_t bestScore = 0;

		for (uint32_t i = 0; i < physicalDeviceCount; i++) {
			VkPhysicalDeviceFeatures features;
			vkGetPhysicalDeviceFeatures(physicalDevices[i], &features);

			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);

			VkPhysicalDeviceMemoryProperties memoryProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDevices[i], &memoryProperties);

			if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) scores[i] += 1000;
			if (features.geometryShader) scores[i] += 100;

			AR_ENGINE_INFO("Device: {}: {}", properties.deviceName, scores[i]);

			if (scores[i] > bestScore) {
				bestScore = scores[i];
				mPhysicalDevice = physicalDevices[i];
			}
		}
		
		delete[] physicalDevices;

		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(mPhysicalDevice, &properties);

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mPhysicalDevice, mSurface, &surfaceCapabilities);

		mMinImageExtent = surfaceCapabilities.minImageExtent;
		mMaxImageExtent = surfaceCapabilities.maxImageExtent;
		
		AR_ENGINE_INFO("Using GPU: {}", properties.deviceName);

		mLimits.Max3DTextureSize = properties.limits.maxImageDimension3D;
		mLimits.MaxTextureSize = properties.limits.maxImageDimension2D;
		mLimits.MaxCubeMapTextureSize = properties.limits.maxImageDimensionCube;
		mLimits.MaxArrayTextureLayers = properties.limits.maxImageArrayLayers;
		
		mLimits.MaxUniformBlocks = properties.limits.maxPerStageDescriptorUniformBuffers;
		mLimits.MaxShaderStorageBlocks = properties.limits.maxPerStageDescriptorStorageBuffers;
		mLimits.MaxTextureImageUnits = properties.limits.maxPerStageDescriptorSampledImages;

		mLimits.MaxUniformBlockSize = properties.limits.maxUniformBufferRange;
		mLimits.MaxStorageBlockSize = properties.limits.maxStorageBufferRange;
		mLimits.MaxVertexAttributes = properties.limits.maxVertexInputAttributes;
		
		mLimits.MaxWorkGroupInvocations = properties.limits.maxComputeWorkGroupInvocations;
		mLimits.MaxWorkGroupCountX = properties.limits.maxComputeWorkGroupCount[0];
		mLimits.MaxWorkGroupCountY = properties.limits.maxComputeWorkGroupCount[1];
		mLimits.MaxWorkGroupCountZ = properties.limits.maxComputeWorkGroupCount[2];
		mLimits.MaxWorkGroupSizeX = properties.limits.maxComputeWorkGroupSize[0];
		mLimits.MaxWorkGroupSizeY = properties.limits.maxComputeWorkGroupSize[1];
		mLimits.MaxWorkGroupSizeZ = properties.limits.maxComputeWorkGroupSize[2];
		
		mLimits.MaxClipDistances = properties.limits.maxClipDistances;
		mLimits.MaxViewportWidth = properties.limits.maxViewportDimensions[0];
		mLimits.MaxViewportHeight = properties.limits.maxViewportDimensions[1];
		mLimits.MaxViewports = properties.limits.maxViewports;
		mLimits.MaxFramebufferWidth = properties.limits.maxFramebufferWidth;
		mLimits.MaxFramebufferHeight = properties.limits.maxFramebufferHeight;
		mLimits.MaxFramebufferLayers = properties.limits.maxFramebufferLayers;
		mLimits.MaxFramebufferSamples = properties.limits.framebufferColorSampleCounts;
		mLimits.MaxDrawBuffers = properties.limits.maxColorAttachments;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, nullptr);

		VkQueueFamilyProperties *queueFamilies = new VkQueueFamilyProperties[queueFamilyCount];
		vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice, &queueFamilyCount, queueFamilies);

		std::vector<bool> queues(queueFamilyCount);

		for (uint32_t j = 0; j < queueFamilyCount; j++) {
			if (queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				mGraphicsQueueFamilyIndex = j;
				queues[j] = true;
			}
			if (queueFamilies[j].queueFlags & VK_QUEUE_COMPUTE_BIT) {
				mComputeQueueFamilyIndex = j;
				queues[j] = true;
			}
			if (queueFamilies[j].queueFlags & VK_QUEUE_TRANSFER_BIT) {
				mTransferQueueFamilyIndex = j;
				queues[j] = true;
			}

			VkBool32 presentSupport = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice, j, mSurface, &presentSupport);
			if (presentSupport) {
				mPresentQueueFamilyIndex = j;
				queues[j] = true;
			}
		}

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		float priority = 1.0f;
		
		for (size_t i = 0; i < queues.size(); i++) {
			if (queues[i]) {
				VkDeviceQueueCreateInfo queueCreateInfo{};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.pNext = nullptr;
				queueCreateInfo.queueFamilyIndex = i;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &priority;

				queueCreateInfos.push_back(queueCreateInfo);
			}
		}

		std::vector<const char *> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		uint32_t availableDeviceExtensionCount = 0;
		AR_VULKAN_CHECK_RESULT(vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &availableDeviceExtensionCount, nullptr));

		VkExtensionProperties *availableDeviceExtensions = new VkExtensionProperties[availableDeviceExtensionCount];
		AR_VULKAN_CHECK_RESULT(vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &availableDeviceExtensionCount, availableDeviceExtensions));

		for (uint32_t i = 0; i < deviceExtensions.size(); i++) {
			bool found = false;
			for (uint32_t j = 0; j < availableDeviceExtensionCount; j++) {
				if (strcmp(deviceExtensions[i], availableDeviceExtensions[j].extensionName) == 0) {
					found = true;
					break;
				}
			}
			AR_ASSERT(found, "Could not find required vulkan device extension: {}\n", deviceExtensions[i]);
		}

		VkDeviceCreateInfo deviceCreateInfo{};
		deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceCreateInfo.pNext = nullptr;
		deviceCreateInfo.flags = 0;
		deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();
		deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
		deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
		deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

		AR_VULKAN_CHECK_RESULT(vkCreateDevice(mPhysicalDevice, &deviceCreateInfo, nullptr, &mLogicalDevice));

		vkGetDeviceQueue(mLogicalDevice, mGraphicsQueueFamilyIndex, 0, &mGraphicsQueue);
		vkGetDeviceQueue(mLogicalDevice, mPresentQueueFamilyIndex, 0, &mPresentQueue);
		vkGetDeviceQueue(mLogicalDevice, mComputeQueueFamilyIndex, 0, &mComputeQueue);
		vkGetDeviceQueue(mLogicalDevice, mTransferQueueFamilyIndex, 0, &mTransferQueue);

		VkCommandPoolCreateInfo commandPoolCreateInfo{};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.pNext = nullptr;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolCreateInfo.queueFamilyIndex = mGraphicsQueueFamilyIndex;

		AR_VULKAN_CHECK_RESULT(vkCreateCommandPool(mLogicalDevice, &commandPoolCreateInfo, nullptr, &mGraphicsCommandPool));

		VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.pNext = nullptr;
		commandBufferAllocateInfo.commandPool = mGraphicsCommandPool;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandBufferCount = 1;

		AR_VULKAN_CHECK_RESULT(vkAllocateCommandBuffers(mLogicalDevice, &commandBufferAllocateInfo, &mGraphicsCommandBuffer));

		uint32_t surfaceFormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &surfaceFormatCount, nullptr);

		VkSurfaceFormatKHR *surfaceFormats = new VkSurfaceFormatKHR[surfaceFormatCount];
		vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice, mSurface, &surfaceFormatCount, surfaceFormats);

		std::vector<VkFormat> requestedFormats = {
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_FORMAT_B8G8R8A8_UNORM,
			VK_FORMAT_R8G8B8A8_SRGB,
		};

		for (uint32_t i = 0; i < surfaceFormatCount; i++) {
			for (VkFormat &format : requestedFormats) {
				if (surfaceFormats[i].format == format) {
					mSurfaceFormat = surfaceFormats[i];
					break;
				}
			}
		}

		delete[] surfaceFormats;

		AR_ASSERT(mSurfaceFormat.format != VK_FORMAT_UNDEFINED, "Could not find a suitable surface format");

		VkExtent2D extent;
		extent.width = Clamp<uint32_t>(mWindow->GetClientSize().X, mMinImageExtent.width, mMaxImageExtent.width);
		extent.height = Clamp<uint32_t>(mWindow->GetClientSize().Y, mMinImageExtent.height, mMaxImageExtent.height);

		mClientSize = mWindow->GetClientSize();

		uint32_t imageAccessQueues[] = {
			mPresentQueueFamilyIndex,
			mGraphicsQueueFamilyIndex
		};

		VkSwapchainCreateInfoKHR swapchainCreateInfo{};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.pNext = nullptr;
		swapchainCreateInfo.surface = mSurface;
		swapchainCreateInfo.minImageCount = AR_IMAGE_COUNT;
		swapchainCreateInfo.imageFormat = mSurfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = mSurfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = { static_cast<uint32_t>(mClientSize.X), static_cast<uint32_t>(mClientSize.Y) };
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = 
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT | 
			VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfo.queueFamilyIndexCount = 2;
		swapchainCreateInfo.pQueueFamilyIndices = imageAccessQueues;
		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		swapchainCreateInfo.clipped = VK_TRUE;
		swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

		AR_VULKAN_CHECK_RESULT(vkCreateSwapchainKHR(mLogicalDevice, &swapchainCreateInfo, nullptr, &mSwapchain));

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.pNext = nullptr;
		fenceCreateInfo.flags = 0;

		AR_VULKAN_CHECK_RESULT(vkCreateFence(mLogicalDevice, &fenceCreateInfo, nullptr, &mPresentFence));
		AR_VULKAN_CHECK_RESULT(vkCreateFence(mLogicalDevice, &fenceCreateInfo, nullptr, &mTransferFence));

		AR_VULKAN_CHECK_RESULT(vkAcquireNextImageKHR(mLogicalDevice, mSwapchain, UINT64_MAX, VK_NULL_HANDLE, mPresentFence, &mNextImageIndex));
		AR_VULKAN_CHECK_RESULT(vkWaitForFences(mLogicalDevice, 1, &mPresentFence, VK_TRUE, UINT64_MAX));
		AR_VULKAN_CHECK_RESULT(vkResetFences(mLogicalDevice, 1, &mPresentFence));

		uint32_t imageCount = 0;
		AR_VULKAN_CHECK_RESULT(vkGetSwapchainImagesKHR(mLogicalDevice, mSwapchain, &imageCount, nullptr));

		VkImage *images = new VkImage[imageCount];
		AR_VULKAN_CHECK_RESULT(vkGetSwapchainImagesKHR(mLogicalDevice, mSwapchain, &imageCount, images));

		AR_ASSERT(imageCount == AR_IMAGE_COUNT, "Swapchain image count does not match expected count: {} != {}", imageCount, AR_IMAGE_COUNT);

		VkImageMemoryBarrier imageBarriers[AR_IMAGE_COUNT] = {};

		VkCommandBufferBeginInfo beginTransferBeginInfo{};
		beginTransferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginTransferBeginInfo.pNext = nullptr;
		beginTransferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		beginTransferBeginInfo.pInheritanceInfo = nullptr;

		AR_VULKAN_CHECK_RESULT(vkBeginCommandBuffer(mGraphicsCommandBuffer, &beginTransferBeginInfo));

		for (uint32_t i = 0; i < imageCount; i++) {
			mImages[i] = images[i];

			VkImageViewCreateInfo imageViewCreateInfo{};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.pNext = nullptr;
			imageViewCreateInfo.flags = 0;
			imageViewCreateInfo.image = mImages[i];
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = mSurfaceFormat.format;
			imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			AR_VULKAN_CHECK_RESULT(vkCreateImageView(mLogicalDevice, &imageViewCreateInfo, nullptr, &mImageViews[i]));

			imageBarriers[i].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageBarriers[i].pNext = nullptr;
			imageBarriers[i].srcAccessMask = 0;
			imageBarriers[i].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			imageBarriers[i].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageBarriers[i].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			imageBarriers[i].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageBarriers[i].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageBarriers[i].image = mImages[i];
			imageBarriers[i].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBarriers[i].subresourceRange.baseMipLevel = 0;
			imageBarriers[i].subresourceRange.levelCount = 1;
			imageBarriers[i].subresourceRange.baseArrayLayer = 0;
			imageBarriers[i].subresourceRange.layerCount = 1;
		}

		vkCmdPipelineBarrier(
			mGraphicsCommandBuffer,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			0,
			0, nullptr,
			0, nullptr,
			AR_IMAGE_COUNT, imageBarriers
		);

		vkEndCommandBuffer(mGraphicsCommandBuffer);

		VkSubmitInfo beginTransferSubmitInfo{};
		beginTransferSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		beginTransferSubmitInfo.pNext = nullptr;
		beginTransferSubmitInfo.waitSemaphoreCount = 0;
		beginTransferSubmitInfo.pWaitSemaphores = nullptr;
		beginTransferSubmitInfo.commandBufferCount = 1;
		beginTransferSubmitInfo.pCommandBuffers = &mGraphicsCommandBuffer;
		beginTransferSubmitInfo.signalSemaphoreCount = 0;
		beginTransferSubmitInfo.pSignalSemaphores = nullptr;
		beginTransferSubmitInfo.pWaitDstStageMask = nullptr;

		vkQueueSubmit(mGraphicsQueue, 1, &beginTransferSubmitInfo, mPresentFence);

		vkWaitForFences(mLogicalDevice, 1, &mPresentFence, VK_TRUE, UINT64_MAX);
		vkResetFences(mLogicalDevice, 1, &mPresentFence);
		
		delete[] images;
	}

	VulkanGraphicsContext::~VulkanGraphicsContext() {
		AR_VULKAN_GET_FUNCTION(vkDestroyDebugUtilsMessengerEXT);
		vkDestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);

		vkDestroyInstance(mInstance, nullptr);
	}

	void VulkanGraphicsContext::Present() {
		if (mClientSize != mWindow->GetClientSize()) {
			VkExtent2D newSize = {
				Clamp<uint32_t>(mWindow->GetClientSize().X, mMinImageExtent.width, mMaxImageExtent.width),
				Clamp<uint32_t>(mWindow->GetClientSize().Y, mMinImageExtent.height, mMaxImageExtent.height)
			};

			AR_VULKAN_DEBUG("Resizing to: {}x{}", newSize.width, newSize.height);
			AR_VULKAN_DEBUG("Current Client Size: {}x{}", (uint32_t)mClientSize.X, (uint32_t)mClientSize.Y);
			AR_VULKAN_DEBUG("Min Extent: {}x{}", mMinImageExtent.width, mMinImageExtent.height);
			AR_VULKAN_DEBUG("Max Extent: {}x{}", mMaxImageExtent.width, mMaxImageExtent.height);

			VkSwapchainKHR oldSwapchain = mSwapchain;

			uint32_t imageAccessQueues[] = {
				mPresentQueueFamilyIndex,
				mGraphicsQueueFamilyIndex
			};

			VkSwapchainCreateInfoKHR swapchainCreateInfo{};
			swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			swapchainCreateInfo.pNext = nullptr;
			swapchainCreateInfo.surface = mSurface;
			swapchainCreateInfo.minImageCount = AR_IMAGE_COUNT;
			swapchainCreateInfo.imageFormat = mSurfaceFormat.format;
			swapchainCreateInfo.imageColorSpace = mSurfaceFormat.colorSpace;
			swapchainCreateInfo.imageExtent = { static_cast<uint32_t>(mClientSize.X), static_cast<uint32_t>(mClientSize.Y) };
			swapchainCreateInfo.imageArrayLayers = 1;
			swapchainCreateInfo.imageUsage = 
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
				VK_IMAGE_USAGE_TRANSFER_SRC_BIT | 
				VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			swapchainCreateInfo.pQueueFamilyIndices = imageAccessQueues;
			swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			swapchainCreateInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			swapchainCreateInfo.clipped = VK_TRUE;
			swapchainCreateInfo.oldSwapchain = oldSwapchain;

			AR_VULKAN_CHECK_RESULT(vkCreateSwapchainKHR(mLogicalDevice, &swapchainCreateInfo, nullptr, &mSwapchain));

			vkDestroySwapchainKHR(mLogicalDevice, oldSwapchain, nullptr);
			AR_VULKAN_CHECK_RESULT(vkAcquireNextImageKHR(mLogicalDevice, mSwapchain, UINT64_MAX, VK_NULL_HANDLE, mPresentFence, &mNextImageIndex));
			AR_VULKAN_CHECK_RESULT(vkWaitForFences(mLogicalDevice, 1, &mPresentFence, VK_TRUE, UINT64_MAX));
			AR_VULKAN_CHECK_RESULT(vkResetFences(mLogicalDevice, 1, &mPresentFence));

			uint32_t imageCount = 0;
			AR_VULKAN_CHECK_RESULT(vkGetSwapchainImagesKHR(mLogicalDevice, mSwapchain, &imageCount, nullptr));

			VkImage *images = new VkImage[imageCount];
			AR_VULKAN_CHECK_RESULT(vkGetSwapchainImagesKHR(mLogicalDevice, mSwapchain, &imageCount, images));

			VkImageMemoryBarrier imageBarriers[AR_IMAGE_COUNT];

			VkCommandBufferBeginInfo beginTransferBeginInfo{};
			beginTransferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginTransferBeginInfo.pNext = nullptr;
			beginTransferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			beginTransferBeginInfo.pInheritanceInfo = nullptr;

			AR_VULKAN_CHECK_RESULT(vkBeginCommandBuffer(mGraphicsCommandBuffer, &beginTransferBeginInfo));

			for (uint32_t i = 0; i < AR_IMAGE_COUNT; i++) {
				mImages[i] = images[i];

				VkImageViewCreateInfo imageViewCreateInfo{};
				imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				imageViewCreateInfo.pNext = nullptr;
				imageViewCreateInfo.flags = 0;
				imageViewCreateInfo.image = mImages[i];
				imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				imageViewCreateInfo.format = mSurfaceFormat.format;
				imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
				imageViewCreateInfo.subresourceRange.levelCount = 1;
				imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
				imageViewCreateInfo.subresourceRange.layerCount = 1;

				AR_VULKAN_CHECK_RESULT(vkCreateImageView(mLogicalDevice, &imageViewCreateInfo, nullptr, &mImageViews[i]));

				imageBarriers[i].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageBarriers[i].pNext = nullptr;
				imageBarriers[i].srcAccessMask = 0;
				imageBarriers[i].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				imageBarriers[i].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				imageBarriers[i].newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				imageBarriers[i].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageBarriers[i].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageBarriers[i].image = mImages[i];
				imageBarriers[i].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageBarriers[i].subresourceRange.baseMipLevel = 0;
				imageBarriers[i].subresourceRange.levelCount = 1;
				imageBarriers[i].subresourceRange.baseArrayLayer = 0;
				imageBarriers[i].subresourceRange.layerCount = 1;
			}

			vkCmdPipelineBarrier(
				mGraphicsCommandBuffer,
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				0,
				0, nullptr,
				0, nullptr,
				AR_IMAGE_COUNT, imageBarriers
			);

			vkEndCommandBuffer(mGraphicsCommandBuffer);

			VkSubmitInfo beginTransferSubmitInfo{};
			beginTransferSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			beginTransferSubmitInfo.pNext = nullptr;
			beginTransferSubmitInfo.waitSemaphoreCount = 0;
			beginTransferSubmitInfo.pWaitSemaphores = nullptr;
			beginTransferSubmitInfo.commandBufferCount = 1;
			beginTransferSubmitInfo.pCommandBuffers = &mGraphicsCommandBuffer;
			beginTransferSubmitInfo.signalSemaphoreCount = 0;
			beginTransferSubmitInfo.pSignalSemaphores = nullptr;
			beginTransferSubmitInfo.pWaitDstStageMask = nullptr;

			vkQueueSubmit(mGraphicsQueue, 1, &beginTransferSubmitInfo, mPresentFence);

			vkWaitForFences(mLogicalDevice, 1, &mPresentFence, VK_TRUE, UINT64_MAX);
			vkResetFences(mLogicalDevice, 1, &mPresentFence);
		}

		VkCommandBufferBeginInfo beginTransferBeginInfo{};
		beginTransferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginTransferBeginInfo.pNext = nullptr;
		beginTransferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		beginTransferBeginInfo.pInheritanceInfo = nullptr;

		AR_VULKAN_CHECK_RESULT(vkBeginCommandBuffer(mGraphicsCommandBuffer, &beginTransferBeginInfo));

		VkImageMemoryBarrier beginTransferImageBarrier{};
		beginTransferImageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		beginTransferImageBarrier.pNext = nullptr;
		beginTransferImageBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		beginTransferImageBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		beginTransferImageBarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		beginTransferImageBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		beginTransferImageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		beginTransferImageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		beginTransferImageBarrier.image = mImages[mNextImageIndex];
		beginTransferImageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		beginTransferImageBarrier.subresourceRange.baseMipLevel = 0;
		beginTransferImageBarrier.subresourceRange.levelCount = 1;
		beginTransferImageBarrier.subresourceRange.baseArrayLayer = 0;
		beginTransferImageBarrier.subresourceRange.layerCount = 1;

		vkCmdPipelineBarrier(
			mGraphicsCommandBuffer,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &beginTransferImageBarrier
		);

		vkEndCommandBuffer(mGraphicsCommandBuffer);

		VkSubmitInfo beginTransferSubmitInfo{};
		beginTransferSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		beginTransferSubmitInfo.pNext = nullptr;
		beginTransferSubmitInfo.waitSemaphoreCount = 0;
		beginTransferSubmitInfo.pWaitSemaphores = nullptr;
		beginTransferSubmitInfo.commandBufferCount = 1;
		beginTransferSubmitInfo.pCommandBuffers = &mGraphicsCommandBuffer;
		beginTransferSubmitInfo.signalSemaphoreCount = 0;
		beginTransferSubmitInfo.pSignalSemaphores = nullptr;
		beginTransferSubmitInfo.pWaitDstStageMask = nullptr;

		vkQueueSubmit(mGraphicsQueue, 1, &beginTransferSubmitInfo, mPresentFence);

		vkWaitForFences(mLogicalDevice, 1, &mPresentFence, VK_TRUE, UINT64_MAX);
		vkResetFences(mLogicalDevice, 1, &mPresentFence);

		uint32_t nextImages[] = { mNextImageIndex };

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.waitSemaphoreCount = 0;
		presentInfo.pWaitSemaphores = nullptr;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &mSwapchain;
		presentInfo.pImageIndices = nextImages;
		presentInfo.pResults = nullptr;

		AR_VULKAN_CHECK_RESULT(vkQueuePresentKHR(mPresentQueue, &presentInfo));
		vkQueueWaitIdle(mPresentQueue);

		VkCommandBufferBeginInfo endTransferBeginInfo{};
		endTransferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		endTransferBeginInfo.pNext = nullptr;
		endTransferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		endTransferBeginInfo.pInheritanceInfo = nullptr;

		AR_VULKAN_CHECK_RESULT(vkBeginCommandBuffer(mGraphicsCommandBuffer, &endTransferBeginInfo));

		VkImageMemoryBarrier endTransferImageBarrier{};
		endTransferImageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		endTransferImageBarrier.pNext = nullptr;
		endTransferImageBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		endTransferImageBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		endTransferImageBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		endTransferImageBarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		endTransferImageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		endTransferImageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		endTransferImageBarrier.image = mImages[mNextImageIndex];
		endTransferImageBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		endTransferImageBarrier.subresourceRange.baseMipLevel = 0;
		endTransferImageBarrier.subresourceRange.levelCount = 1;
		endTransferImageBarrier.subresourceRange.baseArrayLayer = 0;
		endTransferImageBarrier.subresourceRange.layerCount = 1;

		vkCmdPipelineBarrier(
			mGraphicsCommandBuffer,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &endTransferImageBarrier
		);

		vkEndCommandBuffer(mGraphicsCommandBuffer);

		VkSubmitInfo endTransferSubmitInfo{};
		endTransferSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		endTransferSubmitInfo.pNext = nullptr; 
		endTransferSubmitInfo.waitSemaphoreCount = 0;
		endTransferSubmitInfo.pWaitSemaphores = nullptr;
		endTransferSubmitInfo.commandBufferCount = 1;
		endTransferSubmitInfo.pCommandBuffers = &mGraphicsCommandBuffer;
		endTransferSubmitInfo.signalSemaphoreCount = 0;
		endTransferSubmitInfo.pSignalSemaphores = nullptr;
		endTransferSubmitInfo.pWaitDstStageMask = nullptr;

		VkFence waitFences[] = { mTransferFence, mPresentFence };

		vkQueueSubmit(mGraphicsQueue, 1, &endTransferSubmitInfo, mTransferFence);
		vkAcquireNextImageKHR(mLogicalDevice, mSwapchain, UINT64_MAX, VK_NULL_HANDLE, mPresentFence, &mNextImageIndex);

		vkWaitForFences(mLogicalDevice, 2, waitFences, VK_TRUE, UINT64_MAX);
		vkResetFences(mLogicalDevice, 2, waitFences);
	}

}