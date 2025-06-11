#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativePipeline.hpp>
#include <Arcane/Graphics/InputLayout.hpp>

#include "VulkanCore.hpp"
#include "VulkanGraphicsContext.hpp"

namespace Arcane {

	struct VulkanUniformBufferDescriptor {
		uint32_t Binding;
		VkBuffer Buffer;
	};

	struct VulkanCombinedImageSamplerDescriptor {
		uint32_t Binding;
		VkImageView ImageView;
		VkSampler Sampler;
	};

	class VulkanPipeline : public NativePipeline {
	public:
		VulkanPipeline(const Ref<VulkanGraphicsContext> &context, const PipelineInfo &info);
		~VulkanPipeline();

		virtual CullMode GetCullMode() const override { return mCullMode; }
		virtual WindingOrder GetWindingOrder() const override { return mWindingOrder; }
		virtual FillMode GetFillMode() const override { return mFillMode; }
		virtual PrimitiveTopology GetTopology() const override { return mTopology; }
		virtual InputLayout GetLayout() const override { return mLayout; }
		virtual Rect2D GetViewport() const override { return mViewport; }
		virtual Rect2D GetScissor() const override { return mScissor; }
		virtual float GetPolygonOffsetFactor() const override { return mPolygonOffsetFactor; }
		virtual float GetPolygonOffsetUnits() const override { return mPolygonOffsetUnits; }

		virtual uint32_t GetSampleCount() const override { return mSampleCount; }
		virtual uint8_t GetOutputMask() const override { return mOutputMask; }
		virtual float GetLineWidth() const override { return mLineWidth; }
		virtual float GetPointSize() const override { return mPointSize; }

		virtual size_t GetElementSize() const override { return mElementSize; }

		virtual const InputLayout &GetInputLayout() const override { return mLayout; }

		virtual void SetUniformBuffer(uint32_t binding, const Ref<NativeBuffer> &uniformBuffer) override;
		virtual void SetCombinedImageSampler(uint32_t binding, const Ref<NativeTexture> &texture, const Ref<NativeSampler> &sampler) override;

	private:
		Ref<VulkanGraphicsContext> mContext;

		VkPipeline mPipeline;
		
		CullMode mCullMode;
		WindingOrder mWindingOrder;
		FillMode mFillMode;
		PrimitiveTopology mTopology;
		InputLayout mLayout;
		Rect2D mViewport;
		Rect2D mScissor;

		Descriptor *mDescriptors;
		uint32_t mDescriptorCount;
		uint8_t mOutputMask;
		float mLineWidth;
		float mPointSize;

		float mPolygonOffsetFactor;
		float mPolygonOffsetUnits;

		uint32_t mSampleCount;

		size_t mElementSize;

		size_t mUniformBufferDescriptorCount;
		VulkanUniformBufferDescriptor *mUniformBufferDescriptors;
		size_t mCombinedImageSamplerDescriptorCount;
		VulkanCombinedImageSamplerDescriptor *mCombinedImageSamplerDescriptors;
	};

}