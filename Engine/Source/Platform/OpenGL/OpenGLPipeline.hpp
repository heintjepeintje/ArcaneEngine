#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativePipeline.hpp>
#include "OpenGLCore.hpp"
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

		virtual void SetUniformBuffer(uint32_t binding, const Ref<NativeBuffer> &uniformBuffer) override;
		virtual void SetCombinedImageSampler(uint32_t binding, const Ref<NativeTexture> &texture, const Ref<NativeSampler> &sampler) override;
		
		inline std::vector<OpenGLUniformBufferDescriptor> GetUniformBufferDescriptors() const { return mUniformBufferDescriptors; }
		inline std::vector<OpenGLCombinedImageSamplerDescriptor> GetCombinedImageSamplerDescriptors() const { return mCombinedImageSamplerDescriptors; }

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

		uint32_t mSampleCount;

		std::vector<OpenGLUniformBufferDescriptor> mUniformBufferDescriptors;
		std::vector<OpenGLCombinedImageSamplerDescriptor> mCombinedImageSamplerDescriptors;
	};

}