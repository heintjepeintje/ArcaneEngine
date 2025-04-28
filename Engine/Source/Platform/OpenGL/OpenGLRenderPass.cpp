#include "OpenGLRenderPass.hpp"

namespace Arcane {

	OpenGLRenderPass::OpenGLRenderPass(const Ref<OpenGLGraphicsContext> &context, const Ref<OpenGLPipeline> &pipeline, const Attachment *attachments, size_t attachmentCount) 
	: mContext(context), mPipeline(pipeline) {
		AR_PROFILE_FUNCTION();
		mAttachments = new Attachment[attachmentCount];
		memcpy((void*)mAttachments, (void*)attachments, attachmentCount * sizeof(Attachment));
		mAttachmentCount = attachmentCount;
	}

	OpenGLRenderPass::~OpenGLRenderPass() {
		AR_PROFILE_FUNCTION();
		delete[] mAttachments;
	}

}