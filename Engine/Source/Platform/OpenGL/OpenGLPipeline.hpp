#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativePipeline.hpp>

#include "OpenGLCore.hpp"
#include "OpenGLGraphicsContext.hpp"

namespace Arcane {

	struct OpenGLUniformBufferDescriptor {
		uint32_t binding;
		GLuint buffer;
		size_t offset;
		size_t size;
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
		inline GLuint GetProgramPipeline() const { return mProgramPipeline; }

		inline Rect2D GetViewport() const override { return mViewport; }
		inline Rect2D GetScissor() const override { return mScissor; }
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
		inline virtual float GetPolygonOffsetFactor() const override { return mPolygonOffsetFactor; }
		inline virtual float GetPolygonOffsetUnits() const override { return mPolygonOffsetUnits; }
		inline virtual const InputLayout &GetInputLayout() const override { return mLayout; }
		inline virtual uint32_t GetStageFlags() const override { return mStageFlags; }

		virtual void SetUniformBuffer(uint32_t binding, const Ref<NativeBuffer> &uniformBuffer, size_t offset, size_t size) override;
		virtual void SetCombinedImageSampler(uint32_t binding, const Ref<NativeTexture> &texture, const Ref<NativeSampler> &sampler) override;
		
		inline size_t GetUniformBufferDescriptorCount() const { return mUniformBufferDescriptorCount; }
		inline OpenGLUniformBufferDescriptor *GetUniformBufferDescriptors() const { return mUniformBufferDescriptors; }
		inline size_t GetCombinedImageSamplerDescriptorCount() const { return mCombinedImageSamplerDescriptorCount; }
		inline OpenGLCombinedImageSamplerDescriptor *GetCombinedImageSamplerDescriptors() const { return mCombinedImageSamplerDescriptors; }

	private:
		Ref<OpenGLGraphicsContext> mContext;
		GLuint mProgram;
		GLuint mProgramPipeline;
		uint32_t mStageFlags;
		
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
		OpenGLUniformBufferDescriptor *mUniformBufferDescriptors;
		size_t mCombinedImageSamplerDescriptorCount;
		OpenGLCombinedImageSamplerDescriptor *mCombinedImageSamplerDescriptors;
	};

}