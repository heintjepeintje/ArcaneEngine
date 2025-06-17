#include "OpenGLTexture.hpp"

namespace Arcane {

	GLenum ToOpenGLTextureType(TextureType type, uint32_t layers, uint32_t samples) {
		if (type == TextureType::Texture1D) {
			AR_ASSERT(samples == 1, "1 dimensional textures cannot have multisampling\n");
			return layers == 1 ? GL_TEXTURE_1D : GL_TEXTURE_1D_ARRAY;
		} else if (type == TextureType::Texture2D) {
			if (samples > 1) return layers == 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
			else return layers == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_ARRAY;
		} else if (type == TextureType::Texture3D) {
			AR_ASSERT(layers == 1, "3 dimensional textures cannot be in an array\n");
			AR_ASSERT(samples == 1, "3 dimensional textures cannot have multisampling\n");
			return GL_TEXTURE_3D;
		} else if (type == TextureType::CubeMap) {
			AR_ASSERT(layers == 1, "Cube maps cannot be in an array\n");
			AR_ASSERT(samples == 1, "Cube maps cannot have multisampling\n");
			return GL_TEXTURE_CUBE_MAP;
		}
		AR_ASSERT(false, "Unknown texture type: {}\n", (uint32_t)type);
		return GL_NONE;
	}

	GLenum ToOpenGLInternalImageFormat(ImageFormat format) {
		switch (format) {
			case ImageFormat::R8: return GL_R8; 
			case ImageFormat::R8I: return GL_R8I; 
			case ImageFormat::R8U: return GL_R8UI; 
			case ImageFormat::R16: return GL_R16; 
			case ImageFormat::R16I: return GL_R16I; 
			case ImageFormat::R16U: return GL_R16UI; 
			case ImageFormat::R16F: return GL_R16F;
			case ImageFormat::R32I: return GL_R32I; 
			case ImageFormat::R32U: return GL_R32UI; 
			case ImageFormat::R32F: return GL_R32F; 
			case ImageFormat::RG8: return GL_RG8; 
			case ImageFormat::RG8I: return GL_RG8I; 
			case ImageFormat::RG8U: return GL_RG8UI; 
			case ImageFormat::RG16: return GL_RG16; 
			case ImageFormat::RG16I: return GL_RG16I; 
			case ImageFormat::RG16U: return GL_RG16UI; 
			case ImageFormat::RG16F: return GL_RG16F;
			case ImageFormat::RG32I: return GL_RG32I; 
			case ImageFormat::RG32U: return GL_RG32UI; 
			case ImageFormat::RG32F: return GL_RG32F; 
			case ImageFormat::RGB8: return GL_RGB8; 
			case ImageFormat::RGB8I: return GL_RGB8I; 
			case ImageFormat::RGB8U: return GL_RGB8UI; 
			case ImageFormat::RGB16: return GL_RGB16; 
			case ImageFormat::RGB16I: return GL_RGB16I; 
			case ImageFormat::RGB16U: return GL_RGB16UI; 
			case ImageFormat::RGB16F: return GL_RGB16F;
			case ImageFormat::RGB32I: return GL_RGB32I; 
			case ImageFormat::RGB32U: return GL_RGB32UI; 
			case ImageFormat::RGB32F: return GL_RGB32F; 
			case ImageFormat::RGBA8: return GL_RGBA8; 
			case ImageFormat::RGBA8I: return GL_RGBA8I; 
			case ImageFormat::RGBA8U: return GL_RGBA8UI; 
			case ImageFormat::RGBA16: return GL_RGBA16; 
			case ImageFormat::RGBA16I: return GL_RGBA16I;  
			case ImageFormat::RGBA16U: return GL_RGBA16UI;  
			case ImageFormat::RGBA16F: return GL_RGBA16F;
			case ImageFormat::RGBA32I: return GL_RGBA32I;  
			case ImageFormat::RGBA32U: return GL_RGBA32UI;  
			case ImageFormat::RGBA32F: return GL_RGBA32F; 
			case ImageFormat::D16: return GL_DEPTH_COMPONENT16;
			case ImageFormat::D24: return GL_DEPTH_COMPONENT24;
			case ImageFormat::D32: return GL_DEPTH_COMPONENT32;

			case ImageFormat::S8: return GL_STENCIL_INDEX8;
			case ImageFormat::S16: return GL_STENCIL_INDEX16;
			
			case ImageFormat::D24S8: return GL_DEPTH24_STENCIL8;
			case ImageFormat::D32FS8: return GL_DEPTH32F_STENCIL8;
			default: return GL_NONE;
		}
		AR_ASSERT(false, "Image format is unknown: {}\n", (uint32_t)format);
		return GL_NONE;
	}

	GLenum ToOpenGLImageFormat(ImageFormat format) {
		switch (format) {
			case ImageFormat::R8: 
			case ImageFormat::R8I:
			case ImageFormat::R8U:
			case ImageFormat::R16: 
			case ImageFormat::R16I: 
			case ImageFormat::R16U: 
			case ImageFormat::R16F:
			case ImageFormat::R32I: 
			case ImageFormat::R32U: 
			case ImageFormat::R32F: return GL_RED; 

			case ImageFormat::RG8: 
			case ImageFormat::RG8I: 
			case ImageFormat::RG8U: 
			case ImageFormat::RG16: 
			case ImageFormat::RG16I: 
			case ImageFormat::RG16U: 
			case ImageFormat::RG16F:
			case ImageFormat::RG32I: 
			case ImageFormat::RG32U: 
			case ImageFormat::RG32F: return GL_RG; 

			case ImageFormat::RGB8: 
			case ImageFormat::RGB8I:
			case ImageFormat::RGB8U:
			case ImageFormat::RGB16:
			case ImageFormat::RGB16I:
			case ImageFormat::RGB16U: 
			case ImageFormat::RGB16F:
			case ImageFormat::RGB32I:
			case ImageFormat::RGB32U: 
			case ImageFormat::RGB32F: return GL_RGB;

			case ImageFormat::RGBA8:
			case ImageFormat::RGBA8I:
			case ImageFormat::RGBA8U:
			case ImageFormat::RGBA16:
			case ImageFormat::RGBA16I: 
			case ImageFormat::RGBA16U:  
			case ImageFormat::RGBA16F:
			case ImageFormat::RGBA32I: 
			case ImageFormat::RGBA32U:  
			case ImageFormat::RGBA32F: return GL_RGBA;

			case ImageFormat::D16:
			case ImageFormat::D24:
			case ImageFormat::D32:
			case ImageFormat::D32F: return GL_DEPTH_COMPONENT;

			case ImageFormat::S8:
			case ImageFormat::S16: return GL_STENCIL_INDEX;
			
			case ImageFormat::D24S8:
			case ImageFormat::D32FS8: return GL_DEPTH_STENCIL;

			default: return GL_NONE;
		}
		AR_ASSERT(false, "Image format is unknown: {}\n", (uint32_t)format);
		return GL_NONE;
	}

	GLenum GetOpenGLTexelType(ImageFormat format) {
		switch (format) {
			case ImageFormat::R8:
			case ImageFormat::R8U:
			case ImageFormat::RG8:
			case ImageFormat::RG8U:
			case ImageFormat::RGB8:
			case ImageFormat::RGB8U:
			case ImageFormat::RGBA8:
			case ImageFormat::RGBA8U: return GL_UNSIGNED_BYTE;

			case ImageFormat::R8I:
			case ImageFormat::RG8I:
			case ImageFormat::RGB8I:
			case ImageFormat::RGBA8I: return GL_BYTE;

			case ImageFormat::R16:
			case ImageFormat::R16U:
			case ImageFormat::RG16:
			case ImageFormat::RG16U:
			case ImageFormat::RGB16:
			case ImageFormat::RGB16U:
			case ImageFormat::RGBA16:
			case ImageFormat::RGBA16U: return GL_UNSIGNED_SHORT;

			case ImageFormat::R16I:
			case ImageFormat::RG16I:
			case ImageFormat::RGB16I:
			case ImageFormat::RGBA16I: return GL_SHORT;

			case ImageFormat::R16F:
			case ImageFormat::RG16F:
			case ImageFormat::RGB16F:
			case ImageFormat::RGBA16F: return GL_FLOAT;

			case ImageFormat::R32U:
			case ImageFormat::RG32U:
			case ImageFormat::RGB32U:
			case ImageFormat::RGBA32U: return GL_UNSIGNED_INT;

			case ImageFormat::R32I:
			case ImageFormat::RG32I:
			case ImageFormat::RGB32I:
			case ImageFormat::RGBA32I: return GL_INT;

			case ImageFormat::R32F:
			case ImageFormat::RG32F:
			case ImageFormat::RGB32F:
			case ImageFormat::RGBA32F: return GL_FLOAT;

			case ImageFormat::D16: return GL_UNSIGNED_SHORT;
			case ImageFormat::D24: return GL_UNSIGNED_INT;
			case ImageFormat::D24S8: return GL_UNSIGNED_INT_24_8;
			case ImageFormat::D32: return GL_UNSIGNED_INT;
			case ImageFormat::D32F: return GL_FLOAT;
			case ImageFormat::S8: return GL_UNSIGNED_BYTE;
			case ImageFormat::S16: return GL_UNSIGNED_SHORT;
		}
		AR_ASSERT(false, "Image format is unknown: {}\n", (uint32_t)format);
		return GL_NONE;
	}

	OpenGLTexture::OpenGLTexture(const Ref<OpenGLGraphicsContext> &context, const TextureInfo &info) : mContext(context), mWidth(info.Width), mHeight(info.Height), mDepth(info.Depth), mLevels(info.Levels), mLayers(info.Layers), mType(info.Type), mFormat(info.Format), mSamples(info.Samples), mFixedSampleLocations(info.FixedSampleLocations) {
		AR_PROFILE_FUNCTION_GPU_CPU();

		AR_ASSERT(mSamples != 0, "Texture samples cannot be 0");
		AR_ASSERT(mLayers != 0, "Texture layers cannot be 0");
		AR_ASSERT(mLevels != 0, "Texture levels cannot be 0");

		if (mType == TextureType::Texture1D) {
			AR_ASSERT(mWidth != 0, "Texture width cannot be 0");
			AR_ASSERT(
				mWidth <= context->GetGraphicsLimits().MaxTextureSize,
				"Texture is too big (width: {}). Maximum width is: {}\n",
				mWidth,
				context->GetGraphicsLimits().MaxTextureSize
			);
		} else if (mType == TextureType::Texture2D) {
			AR_ASSERT(mWidth != 0, "Texture width cannot be 0");
			AR_ASSERT(mHeight != 0, "Texture height cannot be 0");
			AR_ASSERT(
				mWidth <= mContext->GetGraphicsLimits().MaxTextureSize && 
				mHeight <= mContext->GetGraphicsLimits().MaxTextureSize,
				"Texture is too big ({}x{}). Maximum size is: {}x{}\n",
				mWidth, mHeight,
				mContext->GetGraphicsLimits().MaxTextureSize, mContext->GetGraphicsLimits().MaxTextureSize
			);
		} else if (mType == TextureType::Texture3D) {
			AR_ASSERT(mWidth != 0, "Texture width cannot be 0");
			AR_ASSERT(mHeight != 0, "Texture height cannot be 0");
			AR_ASSERT(mDepth != 0, "Texture depth cannot be 0");
			AR_ASSERT(
				mWidth <= mContext->GetGraphicsLimits().Max3DTextureSize && 
				mHeight <= mContext->GetGraphicsLimits().Max3DTextureSize &&
				mDepth <= mContext->GetGraphicsLimits().Max3DTextureSize,
				"Texture is too big ({}x{}x{}). Maximum size is: {}x{}x{}\n",
				mWidth, mHeight, mDepth,
				mContext->GetGraphicsLimits().Max3DTextureSize, mContext->GetGraphicsLimits().Max3DTextureSize, mContext->GetGraphicsLimits().Max3DTextureSize
			);
		}

		if (IsColorFormat(mFormat)) {
			AR_ASSERT(mSamples <= context->GetGraphicsLimits().MaxColorSamples, "Color texture samples exceeds maximum ({} > {})\n", mSamples, context->GetGraphicsLimits().MaxColorSamples);
		} else if (IsDepthFormat(mFormat)) {
			AR_ASSERT(mSamples <= context->GetGraphicsLimits().MaxDepthSamples, "Depth texture samples exceeds maximum ({} > {})\n", mSamples, context->GetGraphicsLimits().MaxDepthSamples);	
		}

		glCreateTextures(ToOpenGLTextureType(mType, mLayers, mSamples), 1, &mTexture);
		glTextureParameteri(mTexture, GL_TEXTURE_BASE_LEVEL, 0);
		glTextureParameteri(mTexture, GL_TEXTURE_MAX_LEVEL, mLevels - 1);

		const GLenum openglFormat = ToOpenGLInternalImageFormat(mFormat);

		if (mType == TextureType::Texture1D) {
			if (mLayers == 1) glTextureStorage1D(mTexture, mLevels, openglFormat, mWidth);
			else glTextureStorage2D(mTexture, mLevels, openglFormat, mWidth, mLayers);
		} else if (mType == TextureType::Texture2D) {
			if (mSamples == 1 && mLayers == 1) glTextureStorage2D(mTexture, mLevels, openglFormat, mWidth, mHeight);
			else if (mSamples == 1 && mLayers > 1) glTextureStorage3D(mTexture, mLevels, openglFormat, mWidth, mHeight, mLayers);
			else if (mSamples > 1 && mLayers == 1) glTextureStorage2DMultisample(mTexture, mSamples, mLevels, mWidth, mHeight, mFixedSampleLocations);
			else if (mSamples > 1 && mLayers > 1) glTextureStorage3DMultisample(mTexture, mSamples, mLevels, mWidth, mHeight, mLayers, mFixedSampleLocations);
			else AR_ASSERT(false, "Unreachable code");
		} else if (mType == TextureType::Texture3D) {
			glTextureStorage3D(mTexture, mLevels, openglFormat, mWidth, mHeight, mDepth);
		} else {
			AR_ASSERT(false, "Texture type is unknown: {}\n", (uint32_t)mType);
		}
	}

	OpenGLTexture::~OpenGLTexture() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glDeleteTextures(1, &mTexture);
	}

	void OpenGLTexture::SetImage(uint32_t level, uint32_t index, const ImageData &image) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		
		if (mType == TextureType::CubeMap) { AR_ASSERT(index < 6, "Index is out of bounds for cube map texture: {} >= 6", index); }
		else { AR_ASSERT(index < mLayers, "Index is out of bounds for array texture: {} >= {}\n", index, mLayers); }

		AR_ASSERT(level < mLevels, "Texture level out of range: {} >= {}", level, mLevels);
		const GLenum texelType = GetOpenGLTexelType(image.Format);
		const GLenum imageFormat = ToOpenGLImageFormat(image.Format);

		switch (mType) {
			case TextureType::Texture1D:
				glTextureSubImage1D(mTexture, level, 0, image.Width, imageFormat, texelType, image.Data.GetPointer());
				break;
			case TextureType::Texture2D:
				glTextureSubImage2D(mTexture, level, 0, 0, image.Width, image.Height, imageFormat, texelType, image.Data.GetPointer());
				break;
			case TextureType::Texture3D:
				glTextureSubImage3D(mTexture, level, 0, 0, 0, image.Width, image.Height, image.Depth, imageFormat, texelType, image.Data.GetPointer());
				break;
			case TextureType::CubeMap:
				glTextureSubImage3D(mTexture, level, 0, 0, index, image.Width, image.Height, 1, imageFormat, texelType, image.Data.GetPointer());
				break;
		}
	}

	GLenum ToOpenGLSamplerFilterWithMipmapFilter(SamplerFilter filter, SamplerFilter mipmapFilter) {
		if (filter == SamplerFilter::Nearest) {
			switch (mipmapFilter) {
				case SamplerFilter::Nearest: return GL_NEAREST_MIPMAP_NEAREST;
				case SamplerFilter::Linear: return GL_NEAREST_MIPMAP_LINEAR;
			}
			AR_ASSERT(false, "Invalid mipmap sampler unknown: {}\n", (uint32_t)mipmapFilter);
			return GL_NONE;
		} else if (filter == SamplerFilter::Linear) {
			switch (mipmapFilter) {
				case SamplerFilter::Nearest: return GL_LINEAR_MIPMAP_NEAREST;
				case SamplerFilter::Linear: return GL_LINEAR_MIPMAP_LINEAR;
			}
			AR_ASSERT(false, "Mipmap sampler filter is unknown: {}\n", (uint32_t)mipmapFilter);
			return GL_NONE;
		}
		AR_ASSERT(false, "Sampler filter is unknown: {}\n", (uint32_t)filter);
		return GL_NONE;
	}

	GLenum ToOpenGLSamplerFilter(SamplerFilter filter) {
		switch (filter) {
			case SamplerFilter::Nearest: return GL_NEAREST;
			case SamplerFilter::Linear: return GL_LINEAR;
		}
		AR_ASSERT(false, "Sampler filter is unknown: {}\n", (uint32_t)filter);
		return GL_NONE;
	}

	GLenum ToOpenGLSamplerWrap(SamplerWrap wrap) {
		switch (wrap) {
			case SamplerWrap::Repeat: return GL_REPEAT;
			case SamplerWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
			case SamplerWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
			case SamplerWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
		}
		AR_ASSERT(false, "Sampler wrap is unknown: {}\n", (uint32_t)wrap);
		return GL_NONE;
	}

	OpenGLSampler::OpenGLSampler(const Ref<OpenGLGraphicsContext> &context, const SamplerInfo &info) : mContext(context), mMinFilter(info.MinFilter), mMagFilter(info.MagFilter), mMipmapFilter(info.MipmapFilter), mMinLOD(info.MinLOD), mMaxLOD(info.MaxLOD), mBorderColor(info.BorderColor), mWrapS(info.WrapS), mWrapT(info.WrapT), mWrapR(info.WrapR) {
		AR_PROFILE_FUNCTION_GPU_CPU();

		glCreateSamplers(1, &mSampler);
		
		glSamplerParameteri(mSampler, GL_TEXTURE_MIN_FILTER, ToOpenGLSamplerFilterWithMipmapFilter(mMinFilter, mMipmapFilter));
		glSamplerParameteri(mSampler, GL_TEXTURE_MAG_FILTER, ToOpenGLSamplerFilter(mMagFilter));

		glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_S, ToOpenGLSamplerWrap(mWrapS));
		glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_T, ToOpenGLSamplerWrap(mWrapT));
		glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_R, ToOpenGLSamplerWrap(mWrapR));
		
		glSamplerParameterf(mSampler, GL_TEXTURE_MIN_LOD, mMinLOD);
		glSamplerParameterf(mSampler, GL_TEXTURE_MAX_LOD, mMaxLOD);
		
		float border[] = { mBorderColor.R, mBorderColor.G, mBorderColor.B, mBorderColor.A };
		glSamplerParameterfv(mSampler, GL_TEXTURE_BORDER_COLOR, border);
	}

	OpenGLSampler::~OpenGLSampler() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glDeleteSamplers(1, &mSampler);
	}

	void OpenGLTexture::GenerateMipmaps() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glGenerateTextureMipmap(mTexture);
	}

}