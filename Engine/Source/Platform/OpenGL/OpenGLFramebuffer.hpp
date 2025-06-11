#pragma once

#include <Arcane/Core.hpp>
#include <utility>
#include <vector>
#include <Arcane/Native/NativeFramebuffer.hpp>
#include "OpenGL.hpp"
#include "OpenGLGraphicsContext.hpp"

#include "OpenGLTexture.hpp"

namespace Arcane {

	class OpenGLFramebuffer : public NativeFramebuffer {
	public:
		OpenGLFramebuffer(const Ref<OpenGLGraphicsContext> &context, const FramebufferInfo &info);
		~OpenGLFramebuffer();

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual Ref<NativeTexture> GetColorTexture(uint32_t index) override;
		inline virtual uint32_t GetColorAttachmentCount() const override { return mColorAttachmentCount; }
		virtual Ref<NativeTexture> GetDepthTexture() override;
		virtual Ref<NativeTexture> GetStencilTexture() override;

		inline virtual uint32_t GetWidth() const override { return mWidth; }
		inline virtual uint32_t GetHeight() const override { return mHeight; }
		inline virtual uint32_t GetSampleCount() const override { return mSamples; }
		inline virtual bool HasFixedSampleLocations() const override { return mFixedSampleLocations; }

		inline virtual const ImageFormat *GetAttachments() const override { return mAttachments; }
		inline virtual size_t GetAttachmentCount() const override { return mAttachmentCount; } 

		inline GLuint GetOpenGLID() const { return mFramebuffer; }
		
	private:
		Ref<OpenGLGraphicsContext> mContext;
		GLuint mFramebuffer;

		uint32_t mWidth, mHeight;
		uint32_t mSamples;
		bool mFixedSampleLocations;

		const ImageFormat *mAttachments;
		size_t mAttachmentCount;

		Ref<OpenGLTexture> *mColorAttachments;
		size_t mColorAttachmentCount;
		Ref<OpenGLTexture> mDepthAttachment;
		Ref<OpenGLTexture> mStencilAttachment;
	};
}