#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeRenderPass.hpp>
#include "OpenGLGraphicsContext.hpp"
#include "OpenGLPipeline.hpp"

namespace Arcane {

	class OpenGLRenderPass : public NativeRenderPass {
	public:
		OpenGLRenderPass(const Ref<OpenGLGraphicsContext> &context, const Ref<OpenGLPipeline> &pipeline, const ImageFormat *attachments, size_t attachmentCount);
		~OpenGLRenderPass();

		inline virtual Ref<NativePipeline> GetPipeline() const override { return CastRef<NativePipeline>(mPipeline); }
		inline virtual const ImageFormat *GetAttachments() const override { return mAttachments; }
		inline virtual size_t GetAttachmentCount() const override { return mAttachmentCount; }

	private:
		Ref<OpenGLGraphicsContext> mContext;
		Ref<OpenGLPipeline> mPipeline;
		const ImageFormat *mAttachments;
		size_t mAttachmentCount;	
	};

}