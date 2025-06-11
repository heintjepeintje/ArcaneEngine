#include "OpenGLFramebuffer.hpp"

namespace Arcane {

	OpenGLFramebuffer::OpenGLFramebuffer(const Ref<OpenGLGraphicsContext> &context, const FramebufferInfo &info) : mContext(context), mWidth(info.Width), mHeight(info.Height), mSamples(info.Samples), mFixedSampleLocations(info.FixedSampleLocations), mAttachmentCount(info.AttachmentCount) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		
		AR_ASSERT(mWidth != 0 && mHeight != 0, "Framebuffer size cannot be 0\n");
		AR_ASSERT(mSamples <= context->GetGraphicsLimits().MaxFramebufferSamples, "Framebuffer exceeds maximum sample count: %u > %u\n", mSamples, context->GetGraphicsLimits().MaxFramebufferSamples);
		
		AR_ASSERT(
			mWidth <= mContext->GetGraphicsLimits().MaxFramebufferWidth &&
			mHeight <= mContext->GetGraphicsLimits().MaxFramebufferHeight,
			"Framebuffer is too big (%ux%u). Maximum size is %ux%u", 
			mWidth, mHeight,
			mContext->GetGraphicsLimits().MaxFramebufferWidth, mContext->GetGraphicsLimits().MaxFramebufferHeight
		);
		
		AR_ASSERT(mAttachmentCount > 0, "Attachment count must be greater than 0\n");
		AR_ASSERT(mAttachments, "Framebuffer attachments are invalid\n");
		
		glCreateFramebuffers(1, &mFramebuffer);
		
		glNamedFramebufferParameteri(mFramebuffer, GL_FRAMEBUFFER_DEFAULT_WIDTH, mWidth);
		glNamedFramebufferParameteri(mFramebuffer, GL_FRAMEBUFFER_DEFAULT_HEIGHT, mHeight);
		glNamedFramebufferParameteri(mFramebuffer, GL_FRAMEBUFFER_DEFAULT_SAMPLES, mSamples);
		glNamedFramebufferParameteri(mFramebuffer, GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS, mFixedSampleLocations);
		
		uint32_t drawBufferCount = 0;
		
		mAttachments = new ImageFormat[mAttachmentCount];
		std::memcpy((void*)mAttachments, (void*)info.Attachments, mAttachmentCount * sizeof(ImageFormat));
		
		printf("Framebuffer:\n");
		for (uint32_t i = 0; i < mAttachmentCount; i++) {
			if (IsColorFormat(info.Attachments[i])) drawBufferCount++;
		}

		mColorAttachmentCount = drawBufferCount;
		mColorAttachments = new Ref<OpenGLTexture>[mColorAttachmentCount];
		
		AR_ASSERT(drawBufferCount <= mContext->GetGraphicsLimits().MaxDrawBuffers, "Framebuffer exceeds maximum draw buffer count: %u > %u\n", drawBufferCount, mContext->GetGraphicsLimits().MaxDrawBuffers);
		
		uint32_t nextColorAttachmentIndex = 0;
		
		for (uint32_t i = 0; i < mAttachmentCount; i++) {
			TextureInfo textureInfo;
			textureInfo.Type = TextureType::Texture2D;
			textureInfo.Samples = mSamples;
			textureInfo.FixedSampleLocations = mFixedSampleLocations;
			textureInfo.Format = mAttachments[i];
			textureInfo.Levels = 1;
			textureInfo.Layers = 1;
			textureInfo.Width = mWidth;
			textureInfo.Height = mHeight;
			textureInfo.Depth = 1;

			Ref<OpenGLTexture> texture = CreateRef<OpenGLTexture>(mContext, textureInfo);

			if (IsDepthFormat(mAttachments[i])) {
				mDepthAttachment = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_DEPTH_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else if (IsStencilFormat(mAttachments[i])) {
				mStencilAttachment = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_STENCIL_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else if (IsDepthFormat(mAttachments[i]) && IsStencilFormat(mAttachments[i])) {
				mDepthAttachment = texture;
				mStencilAttachment = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_DEPTH_STENCIL_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else {
				mColorAttachments[nextColorAttachmentIndex] = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_COLOR_ATTACHMENT0 + nextColorAttachmentIndex, texture->GetOpenGLID(), 0);
				nextColorAttachmentIndex++;
			}
		}

		if (drawBufferCount == 0) {
			glNamedFramebufferDrawBuffer(mFramebuffer, GL_NONE);
		} else {
			GLenum *drawBuffers = new GLenum[drawBufferCount];
			for (uint32_t i = 0; i < drawBufferCount; i++) {
				drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
			}
			glNamedFramebufferDrawBuffers(mFramebuffer, drawBufferCount, drawBuffers);
			delete[] drawBuffers;
		}

		GLenum status = glCheckNamedFramebufferStatus(mFramebuffer, GL_FRAMEBUFFER);
		AR_ASSERT(status != GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, "A framebuffer attachment is invalid\n");
		AR_ASSERT(status != GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, "Framebuffer must have at least 1 attachment\n");
		AR_ASSERT(status != GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER, "A framebuffer draw buffer is invalid\n");
		AR_ASSERT(status != GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER, "A framebuffer read buffer is invalid\n");
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		delete[] mAttachments;
		glDeleteFramebuffers(1, &mFramebuffer);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		if (mWidth == width && mHeight == height) return;

		mWidth = width;
		mHeight = height;
		
		AR_ASSERT(mWidth != 0 && mHeight != 0, "Framebuffer size cannot be 0\n");
		AR_ASSERT(
			mWidth <= mContext->GetGraphicsLimits().MaxFramebufferWidth &&
			mHeight <= mContext->GetGraphicsLimits().MaxFramebufferHeight,
			"Framebuffer is too big (%ux%u). Maximum size is %ux%u", 
			mWidth, mHeight,
			mContext->GetGraphicsLimits().MaxFramebufferWidth, mContext->GetGraphicsLimits().MaxFramebufferHeight
		);

		uint32_t nextColorAttachmentIndex = 0;

		for (uint32_t i = 0; i < mAttachmentCount; i++) {
			TextureInfo textureInfo;
			textureInfo.Type = TextureType::Texture2D;
			textureInfo.Width = mWidth;
			textureInfo.Height = mHeight;
			textureInfo.Format = mAttachments[i];
			textureInfo.Levels = 1;
			textureInfo.Layers = 1;
			textureInfo.Depth = 1;
			textureInfo.Samples = mSamples;
			textureInfo.FixedSampleLocations = mFixedSampleLocations;

			Ref<OpenGLTexture> texture = CreateRef<OpenGLTexture>(mContext, textureInfo);

			if (IsDepthFormat(mAttachments[i])) {
				mDepthAttachment = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_DEPTH_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else if (IsStencilFormat(mAttachments[i])) {
				mStencilAttachment = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_STENCIL_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else if (IsDepthFormat(mAttachments[i]) && IsStencilFormat(mAttachments[i])) {
				mDepthAttachment = texture;
				mStencilAttachment = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_DEPTH_STENCIL_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else {
				mColorAttachments[nextColorAttachmentIndex] = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_COLOR_ATTACHMENT0 + nextColorAttachmentIndex, texture->GetOpenGLID(), 0);
				nextColorAttachmentIndex++;
			}
		}

		GLenum status = glCheckNamedFramebufferStatus(mFramebuffer, GL_FRAMEBUFFER);
		AR_ASSERT(status != GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, "A framebuffer attachment is invalid\n");
		AR_ASSERT(status != GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, "Framebuffer must have at least 1 attachment\n");
		AR_ASSERT(status != GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER, "A framebuffer draw buffer is invalid\n");
		AR_ASSERT(status != GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER, "A framebuffer read buffer is invalid\n");
	}

	Ref<NativeTexture> OpenGLFramebuffer::GetColorTexture(uint32_t index) {
		AR_ASSERT(index < mColorAttachmentCount, "Framebuffer color texture index is out of range: %u >= %u\n", index, mColorAttachmentCount);
		AR_ASSERT(mColorAttachments[index], "Framebufer color texture is invalid at index: %u\n", index);
		return CastRef<NativeTexture>(mColorAttachments[index]);
	}

	Ref<NativeTexture> OpenGLFramebuffer::GetDepthTexture() {
		AR_ASSERT(mDepthAttachment, "Framebuffer depth texture is invalid\n");
		return CastRef<NativeTexture>(mDepthAttachment);
	}

	Ref<NativeTexture> OpenGLFramebuffer::GetStencilTexture() {
		AR_ASSERT(mStencilAttachment, "Framebuffer stencil texture is invalid\n");
		return CastRef<NativeTexture>(mStencilAttachment);
	}

}