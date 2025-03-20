#pragma once

#include <Arcane/Core.hpp>
#include <utility>
#include <vector>
#include <Arcane/Native/NativeFramebuffer.hpp>
#include "OpenGLCore.hpp"
#include "OpenGLGraphicsContext.hpp"

#include "OpenGLTexture.hpp"

namespace Arcane {

	class OpenGLFramebuffer : public NativeFramebuffer {
	public:
		OpenGLFramebuffer(const Ref<OpenGLGraphicsContext> &context, const FramebufferInfo &info);
		~OpenGLFramebuffer();

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual Ref<NativeTexture> GetColorBuffer(uint32_t index) override;
		inline virtual uint32_t GetColorBufferCount() const override { return mColorAttachments.size(); }
		virtual Ref<NativeTexture> GetDepthBuffer() override;
		virtual Ref<NativeTexture> GetStencilBuffer() override;

		inline virtual uint32_t GetWidth() const override { return mWidth; }
		inline virtual uint32_t GetHeight() const override { return mHeight; }

		inline virtual const Attachment *GetAttachments() const override { return mAttachments; }
		inline virtual size_t GetAttachmentCount() const override { return mAttachmentCount; } 

		inline GLuint GetOpenGLID() const { return mFramebuffer; }
		
	private:
		Ref<OpenGLGraphicsContext> mContext;
		GLuint mFramebuffer;

		uint32_t mWidth, mHeight;

		const Attachment *mAttachments;
		size_t mAttachmentCount;

		std::vector<Ref<OpenGLTexture>> mColorAttachments;
		Ref<OpenGLTexture> mDepthAttachment;
		Ref<OpenGLTexture> mStencilAttachment;
	};
}