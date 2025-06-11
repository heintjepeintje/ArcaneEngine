#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeGraphicsContext.hpp>
#include "VulkanCore.hpp"

#define AR_IMAGE_COUNT 2

namespace Arcane {

	class VulkanGraphicsContext : public NativeGraphicsContext {
	public:
		VulkanGraphicsContext(const Ref<NativeWindow> &window);
		~VulkanGraphicsContext();

		virtual void Present() override;
		virtual uint32_t GetVersionMajor() const { return 1; }
		virtual uint32_t GetVersionMinor() const { return 3;}
		virtual uint32_t GetPatchLevel() const { return 0; }
		virtual const char *GetDeviceName() const { return mDeviceName; }
		virtual Ref<NativeWindow> GetWindow() const { return mWindow; }
		virtual GraphicsAPI GetGraphicsAPI() const { return GraphicsAPI::Vulkan; }
		virtual const GraphicsLimits &GetGraphicsLimits() const { return mLimits; }

		inline VkInstance GetInstance() const { return mInstance; }
		inline VkDebugUtilsMessengerEXT GetDebugMessenger() const { return mDebugMessenger; }
		inline VkSurfaceKHR GetSurface() const { return mSurface; }
		inline VkPhysicalDevice GetPhysicalDevice() const { return mPhysicalDevice; }
		inline VkDevice GetLogicalDevice() const { return mLogicalDevice; }
		inline VkSwapchainKHR GetSwapchain() const { return mSwapchain; }

		inline VkQueue GetGraphicsQueue() const { return mGraphicsQueue; }
		inline VkQueue GetPresentQueue() const { return mPresentQueue; }
		inline VkQueue GetComputeQueue() const { return mComputeQueue; }
		inline VkQueue GetTransferQueue() const { return mTransferQueue; }
	private:
		Ref<NativeWindow> mWindow;
		Vector2 mClientSize;
		GraphicsLimits mLimits;

		VkInstance mInstance;
		VkDebugUtilsMessengerEXT mDebugMessenger;
		VkSurfaceKHR mSurface;
		VkPhysicalDevice mPhysicalDevice;
		VkDevice mLogicalDevice;
		VkSurfaceFormatKHR mSurfaceFormat;
		VkSwapchainKHR mSwapchain;

		VkExtent2D mMinImageExtent;
		VkExtent2D mMaxImageExtent;

		VkImage mImages[AR_IMAGE_COUNT];
		VkFence mTransferFence, mPresentFence;
		VkImageView mImageViews[AR_IMAGE_COUNT];
		VkFramebuffer mFramebuffers[AR_IMAGE_COUNT];

		VkCommandPool mGraphicsCommandPool;
		VkCommandBuffer mGraphicsCommandBuffer;

		const char *mDeviceName = nullptr;

		uint32_t mNextImageIndex = 0;

		uint32_t mGraphicsQueueFamilyIndex;
		uint32_t mPresentQueueFamilyIndex;
		uint32_t mComputeQueueFamilyIndex;
		uint32_t mTransferQueueFamilyIndex;

		VkQueue mGraphicsQueue;
		VkQueue mPresentQueue;
		VkQueue mComputeQueue;
		VkQueue mTransferQueue;
	};

}