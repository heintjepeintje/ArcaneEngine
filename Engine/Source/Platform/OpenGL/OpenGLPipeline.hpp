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

	class OpenGLPipeline : public NativePipeline {
	public:
		OpenGLPipeline(const std::shared_ptr<OpenGLGraphicsContext> &context, const PipelineInfo &info);
		~OpenGLPipeline();

		inline GLuint GetShaderProgram() const { return mProgram; }
		inline Rect2D GetViewport() const { return mViewport; }
		inline Rect2D GetScissor() const { return mScissor; }

		inline virtual CullMode GetCullMode() const override { return mCullMode; }
		inline virtual WindingOrder GetWindingOrder() const override { return mWindingOrder; }
		inline virtual FillMode GetFillMode() const override { return mFillMode; }
		inline virtual PrimitiveTopology GetTopology() const override { return mTopology; }
		inline virtual InputLayout GetLayout() const override { return mLayout; }

		virtual void SetDescriptor(uint32_t binding, const std::shared_ptr<NativeBuffer> &uniformBuffer) override;
		inline std::vector<OpenGLUniformBufferDescriptor> GetUniformBufferDescriptors() const { return mUniformBufferDescriptors; }

	private:
		std::shared_ptr<OpenGLGraphicsContext> mContext;
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

		std::vector<OpenGLUniformBufferDescriptor> mUniformBufferDescriptors;
	};

}