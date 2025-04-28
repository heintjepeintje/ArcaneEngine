#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativePipeline.hpp>
#include "OpenGL.hpp"
#include "OpenGLGraphicsContext.hpp"

#include <vector>

namespace Arcane {

	struct OpenGLUniformBufferDescriptor {
		uint32_t binding;
		GLuint buffer;
	};

	struct OpenGLCombinedImageSamplerDescriptor {
		uint32_t binding;
		GLuint texture;
		GLuint sampler;
	};

	class OpenGLPipeline : public NativePipeline {
	public:
		OpenGLPipeline(const Ref<OpenGLGraphicsContext> &context, const PipelineInfo &info);
		~OpenGLPipeline();

		inline GLuint GetShaderProgram() const { return mProgram; }
		inline Rect2D GetViewport() const { return mViewport; }
		inline Rect2D GetScissor() const { return mScissor; }

		inline virtual CullMode GetCullMode() const override { return mCullMode; }
		inline virtual WindingOrder GetWindingOrder() const override { return mWindingOrder; }
		inline virtual FillMode GetFillMode() const override { return mFillMode; }
		inline virtual PrimitiveTopology GetTopology() const override { return mTopology; }
		inline virtual InputLayout GetLayout() const override { return mLayout; }
		inline virtual uint32_t GetSampleCount() const override { return mSampleCount; }
		inline virtual uint8_t GetOutputMask() const override { return mOutputMask; }
		inline virtual float GetLineWidth() const override { return mLineWidth; }
		inline virtual float GetPointSize() const override { return mPointSize; }
		inline virtual size_t GetElementSize() const override { return mElementSize; }

		virtual void SetUniformBuffer(uint32_t binding, const Ref<NativeBuffer> &uniformBuffer) override;
		virtual void SetCombinedImageSampler(uint32_t binding, const Ref<NativeTexture> &texture, const Ref<NativeSampler> &sampler) override;
		
		inline size_t GetUniformBufferDescriptorCount() const { return mUniformBufferDescriptorCount; }
		inline OpenGLUniformBufferDescriptor *GetUniformBufferDescriptors() const { return mUniformBufferDescriptors; }
		inline size_t GetCombinedImageSamplerDescriptorCount() const { return mCombinedImageSamplerDescriptorCount; }
		inline OpenGLCombinedImageSamplerDescriptor *GetCombinedImageSamplerDescriptors() const { return mCombinedImageSamplerDescriptors; }

	private:
		Ref<OpenGLGraphicsContext> mContext;
		GLuint mProgram;
		
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

		uint32_t mSampleCount;

		size_t mElementSize;

		size_t mUniformBufferDescriptorCount;
		OpenGLUniformBufferDescriptor *mUniformBufferDescriptors;
		size_t mCombinedImageSamplerDescriptorCount;
		OpenGLCombinedImageSamplerDescriptor *mCombinedImageSamplerDescriptors;
	};

}