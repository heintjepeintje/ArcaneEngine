#include "OpenGLRenderPass.hpp"

namespace Arcane {

	OpenGLRenderPass::OpenGLRenderPass(const Ref<OpenGLGraphicsContext> &context, const Ref<OpenGLPipeline> &pipeline, const ImageFormat *attachments, size_t attachmentCount) 
	: mContext(context), mPipeline(pipeline) {
		AR_PROFILE_FUNCTION();
		mAttachments = new ImageFormat[attachmentCount];
		memcpy((void*)mAttachments, (void*)attachments, attachmentCount * sizeof(ImageFormat));
		mAttachmentCount = attachmentCount;
	}

	OpenGLRenderPass::~OpenGLRenderPass() {
		AR_PROFILE_FUNCTION();
		delete[] mAttachments;
	}

}