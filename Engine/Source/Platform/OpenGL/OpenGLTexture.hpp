#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeTexture.hpp>

#include "OpenGLCore.hpp"
#include "OpenGLGraphicsContext.hpp"

namespace Arcane {

	class OpenGLTexture : public NativeTexture {
	public:
		OpenGLTexture(const Ref<OpenGLGraphicsContext> &context, const TextureInfo &info);
		~OpenGLTexture();

		virtual void SetImage(uint32_t level, uint32_t index, const ImageData &image) override;
		virtual void GenerateMipmaps() override;

		inline virtual ImageFormat GetFormat() const override { return mFormat; }
		inline virtual TextureType GetType() const override { return mType; }
		inline virtual uint32_t GetWidth() const override { return mWidth; }
		inline virtual uint32_t GetHeight() const override { return mHeight; }
		inline virtual uint32_t GetDepth() const override { return mDepth; }
		inline virtual uint32_t GetLevels() const override { return mLevels; }
		inline virtual uint32_t GetLayers() const override { return mLayers; }
		inline virtual uint32_t GetSampleCount() const override { return mSamples; }
		inline virtual bool HasFixedSampleLocations() const override { return mFixedSampleLocations; }

		inline GLuint GetOpenGLID() const { return mTexture; }
	private:
		Ref<OpenGLGraphicsContext> mContext;
		GLuint mTexture;
		TextureType mType;
		ImageFormat mFormat;
		uint32_t mWidth, mHeight, mDepth;
		uint32_t mLevels;
		uint32_t mLayers;
		uint32_t mSamples;
		bool mFixedSampleLocations;
	};

	class OpenGLSampler : public NativeSampler {
	public:
		OpenGLSampler(const Ref<OpenGLGraphicsContext> &context, const SamplerInfo &info);
		~OpenGLSampler();

		virtual SamplerFilter GetMinFilter() const { return mMinFilter; }
		virtual SamplerFilter GetMagFilter() const { return mMagFilter; }
		virtual SamplerFilter GetMipmapFilter() const { return mMipmapFilter; }
		virtual float GetMinLOD() const { return mMinLOD; }
		virtual float GetMaxLOD() const { return mMaxLOD; }
		virtual Color GetBorderColor() const { return mBorderColor; }
		virtual SamplerWrap GetWrapS() const { return mWrapS; }
		virtual SamplerWrap GetWrapT() const { return mWrapT; }
		virtual SamplerWrap GetWrapR() const { return mWrapR; }

		inline GLuint GetOpenGLID() const { return mSampler; }

	private:
		Ref<OpenGLGraphicsContext> mContext;
		SamplerFilter mMinFilter, mMagFilter, mMipmapFilter;
		float mMinLOD, mMaxLOD;
		Color mBorderColor;
		SamplerWrap mWrapS, mWrapT, mWrapR;
		GLuint mSampler;
	};

}