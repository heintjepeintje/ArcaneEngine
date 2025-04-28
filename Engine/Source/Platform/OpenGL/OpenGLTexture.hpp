#pragma once

#include "OpenGL.hpp"
#include "OpenGLGraphicsContext.hpp"
#include <Arcane/Native/NativeTexture.hpp>

namespace Arcane {

	class OpenGLTexture : public NativeTexture {
	public:
		OpenGLTexture(const Ref<OpenGLGraphicsContext> &context, const TextureInfo &info);
		~OpenGLTexture();

		virtual void SetImage(uint32_t level, const Image &image) override;
		virtual void GenerateMipmaps() override;
		virtual void Resize(uint32_t width, uint32_t height, uint32_t depth) override;

		virtual ImageFormat GetFormat() override { return mFormat; }
		virtual uint32_t GetWidth() override { return mWidth; }
		virtual uint32_t GetHeight() override { return mHeight; }
		virtual uint32_t GetDepth() override { return mDepth; }
		virtual uint32_t GetLevels() override { return mLevels; }
		virtual ImageType GetType() override { return mType; }

		inline GLuint GetOpenGLID() const { return mTexture; }
	private:
		Ref<OpenGLGraphicsContext> mContext;
		GLuint mTexture;
		ImageType mType;
		ImageFormat mFormat;
		uint32_t mWidth, mHeight, mDepth;
		uint32_t mLevels;
		uint32_t mSamples;
	};

	class OpenGLSampler : public NativeSampler {
	public:
		OpenGLSampler(const Ref<OpenGLGraphicsContext> &context, const SamplerInfo &info);
		~OpenGLSampler();

		inline GLuint GetOpenGLID() const { return mSampler; }

	private:
		Ref<OpenGLGraphicsContext> mContext;
		GLuint mSampler;
	};

}