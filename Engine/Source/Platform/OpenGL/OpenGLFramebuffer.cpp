#include "OpenGLFramebuffer.hpp"

namespace Arcane {

	OpenGLFramebuffer::OpenGLFramebuffer(const Ref<OpenGLGraphicsContext> &context, const FramebufferInfo &info) : mContext(context) {
		mWidth = info.Width;
		mHeight = info.Height;

		mAttachmentCount = info.AttachmentCount;
		mAttachments = new Attachment[mAttachmentCount];
		memcpy((void*)mAttachments, (void*)info.Attachments, sizeof(Attachment) * mAttachmentCount);

		glCreateFramebuffers(1, &mFramebuffer);

		uint32_t colorAttachmentIndex = 0;
	
		for (uint32_t i = 0; i < info.AttachmentCount; i++) {
			Attachment attachment = info.Attachments[i];

			TextureInfo info;
			info.Type = ImageType::Texture2D;
			info.Samples = attachment.Samples;
			info.Format = attachment.Format;
			info.Levels = 1;
			info.Width = mWidth;
			info.Height = mHeight;
			info.Depth = 1;

			Ref<OpenGLTexture> texture = CreateRef<OpenGLTexture>(context, info);

			if (attachment.Type == AttachmentType::Depth) {
				mDepthAttachment = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_DEPTH_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else if (attachment.Type == AttachmentType::Stencil) {
				mStencilAttachment = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_STENCIL_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else if (attachment.Type == AttachmentType::DepthStencil) {
				mDepthAttachment = texture;
				mStencilAttachment = texture;
				glNamedFramebufferTexture(mFramebuffer, GL_DEPTH_STENCIL_ATTACHMENT, texture->GetOpenGLID(), 0);
			} else if (attachment.Type == AttachmentType::Color) {
				mColorAttachments.push_back(texture);
				glNamedFramebufferTexture(mFramebuffer, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex, texture->GetOpenGLID(), 0);
				colorAttachmentIndex++;
			} else {
				AR_ASSERT(false, "Invalid attachment type!");
			}

			GLenum status = glCheckNamedFramebufferStatus(mFramebuffer, GL_FRAMEBUFFER);
			AR_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete! (%u)", status);
		}
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		glDeleteFramebuffers(1, &mFramebuffer);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
		if (width == mWidth && height == mHeight) return;
		mWidth = width;
		mHeight = height;
		
		for (Ref<OpenGLTexture> &texture : mColorAttachments) {
			texture->Resize(width, height);
			glNamedFramebufferTexture(mFramebuffer, GL_COLOR_ATTACHMENT0, texture->GetOpenGLID(), 0);
		}

		if (mDepthAttachment == mStencilAttachment && (mDepthAttachment || mStencilAttachment)) {
			mDepthAttachment->Resize(width, height);
			glNamedFramebufferTexture(mFramebuffer, GL_DEPTH_STENCIL_ATTACHMENT, mDepthAttachment->GetOpenGLID(), 0);
		} else {
			if (mDepthAttachment) {
				mDepthAttachment->Resize(width, height);
				glNamedFramebufferTexture(mFramebuffer, GL_DEPTH_ATTACHMENT, mDepthAttachment->GetOpenGLID(), 0);
			}

			if (mStencilAttachment) {
				mStencilAttachment->Resize(width, height);
				glNamedFramebufferTexture(mFramebuffer, GL_STENCIL_ATTACHMENT, mStencilAttachment->GetOpenGLID(), 0);
			}
		}
	}

	Ref<NativeTexture> OpenGLFramebuffer::GetColorBuffer(uint32_t index) {
		return CastRef<NativeTexture>(mColorAttachments[index]);
	}

	Ref<NativeTexture> OpenGLFramebuffer::GetDepthBuffer() {
		return CastRef<NativeTexture>(mDepthAttachment);
	}

	Ref<NativeTexture> OpenGLFramebuffer::GetStencilBuffer() {
		return CastRef<NativeTexture>(mStencilAttachment);
	}

}