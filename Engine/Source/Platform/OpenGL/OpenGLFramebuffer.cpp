#include "OpenGLFramebuffer.hpp"

namespace Arcane {

	OpenGLFramebuffer::OpenGLFramebuffer(const Ref<OpenGLGraphicsContext> &context, const FramebufferInfo &info) : mContext(context), mWidth(info.Width), mHeight(info.Height), mSamples(info.Samples), mFixedSampleLocations(info.FixedSampleLocations), mAttachmentCount(info.AttachmentCount) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		
		AR_ASSERT(mWidth != 0 && mHeight != 0, "Framebuffer size cannot be 0\n");
		AR_ASSERT(mSamples <= context->GetGraphicsLimits().MaxFramebufferSamples, "Framebuffer exceeds maximum sample count: {} > {}\n", mSamples, context->GetGraphicsLimits().MaxFramebufferSamples);
		
		AR_ASSERT(
			mWidth <= mContext->GetGraphicsLimits().MaxFramebufferWidth &&
			mHeight <= mContext->GetGraphicsLimits().MaxFramebufferHeight,
			"Framebuffer is too big ({}x{}). Maximum size is {}x{}",
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

		for (uint32_t i = 0; i < mAttachmentCount; i++) {
			if (IsColorFormat(mAttachments[i])) drawBufferCount++;
		}

		mColorTextureCount = drawBufferCount;
		mColorTextures = new Ref<OpenGLTexture>[mColorTextureCount];
		
		AR_ASSERT(drawBufferCount <= mContext->GetGraphicsLimits().MaxDrawBuffers, "Framebuffer exceeds maximum draw buffer count: {} > {}\n", drawBufferCount, mContext->GetGraphicsLimits().MaxDrawBuffers);
		
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
				mDepthTexture = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_DEPTH_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else if (IsStencilFormat(mAttachments[i])) {
				mStencilTexture = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_STENCIL_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else if (IsDepthFormat(mAttachments[i]) && IsStencilFormat(mAttachments[i])) {
				mDepthTexture = texture;
				mStencilTexture = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_DEPTH_STENCIL_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else {
				mColorTextures[nextColorAttachmentIndex] = texture;
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
		Destroy();
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
			"Framebuffer is too big ({}x{}). Maximum size is {}x{}", 
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
				mDepthTexture = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_DEPTH_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else if (IsStencilFormat(mAttachments[i])) {
				mStencilTexture = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_STENCIL_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else if (IsDepthFormat(mAttachments[i]) && IsStencilFormat(mAttachments[i])) {
				mDepthTexture = texture;
				mStencilTexture = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_DEPTH_STENCIL_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else {
				mColorTextures[nextColorAttachmentIndex] = texture;
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

	bool OpenGLFramebuffer::IsValid() const {
		AR_PROFILE_FUNCTION_GPU_CPU();
		return glIsFramebuffer(mFramebuffer) && glCheckNamedFramebufferStatus(mFramebuffer, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

	void OpenGLFramebuffer::Destroy() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		delete[] mAttachments;
		delete[] mColorTextures;
		glDeleteFramebuffers(1, &mFramebuffer);
		mFramebuffer = 0;
	}

	Ref<NativeTexture> OpenGLFramebuffer::GetColorTexture(uint32_t index) {
		AR_ASSERT(index < mColorTextureCount, "Framebuffer color texture index is out of range: {} >= {}\n", index, mColorTextureCount);
		AR_ASSERT(mColorTextures[index], "Framebufer color texture is invalid at index: {}\n", index);
		return CastRef<NativeTexture>(mColorTextures[index]);
	}

	Ref<NativeTexture> OpenGLFramebuffer::GetDepthTexture() {
		AR_ASSERT(mDepthTexture.IsValid(), "Framebuffer depth texture is invalid\n");
		return CastRef<NativeTexture>(mDepthTexture);
	}

	Ref<NativeTexture> OpenGLFramebuffer::GetStencilTexture() {
		AR_ASSERT(mStencilTexture.IsValid(), "Framebuffer stencil texture is invalid\n");
		return CastRef<NativeTexture>(mStencilTexture);
	}

}