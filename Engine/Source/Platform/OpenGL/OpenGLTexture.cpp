#include "OpenGLTexture.hpp"

namespace Arcane {

	GLenum ToOpenGLImageType(ImageType type, uint32_t samples) {
		if (samples == 1) {
			switch (type) {
				case ImageType::Texture1D: return GL_TEXTURE_1D;
				case ImageType::Texture1DArray: return GL_TEXTURE_1D_ARRAY;
				case ImageType::Texture2D: return GL_TEXTURE_2D;
				case ImageType::Texture2DArray: return GL_TEXTURE_2D_ARRAY;
				case ImageType::Texture3D: return GL_TEXTURE_3D;
				case ImageType::TextureCube: return GL_TEXTURE_CUBE_MAP;
				default: return GL_NONE;
			}
		} else {
			switch (type) {
				case ImageType::Texture2D: return GL_TEXTURE_2D_MULTISAMPLE;
				case ImageType::Texture2DArray: return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
				default: return GL_NONE;
			}
		}
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
			default: return GL_NONE;
		}
	}

	size_t GetFormatTexelSize(ImageFormat format) {
		switch (format) {
			case ImageFormat::R8: return sizeof(uint8_t);
			case ImageFormat::R8I: return sizeof(uint8_t);
			case ImageFormat::R8U: return sizeof(uint8_t); 
			case ImageFormat::R16: return sizeof(uint16_t); 
			case ImageFormat::R16I: return sizeof(uint16_t); 
			case ImageFormat::R16U: return sizeof(uint16_t); 
			case ImageFormat::R16F: return sizeof(uint16_t);
			case ImageFormat::R32I: return sizeof(uint32_t);
			case ImageFormat::R32U: return sizeof(uint32_t);
			case ImageFormat::R32F: return sizeof(uint32_t);
			case ImageFormat::RG8: return 2 * sizeof(uint8_t);
			case ImageFormat::RG8I: return 2 * sizeof(uint8_t);
			case ImageFormat::RG8U: return 2 * sizeof(uint8_t); 
			case ImageFormat::RG16: return 2 * sizeof(uint16_t);
			case ImageFormat::RG16I: return 2 * sizeof(uint16_t);
			case ImageFormat::RG16U: return 2 * sizeof(uint16_t);
			case ImageFormat::RG16F: return 2 * sizeof(uint16_t);
			case ImageFormat::RG32I: return 2 * sizeof(uint32_t);
			case ImageFormat::RG32U: return 2 * sizeof(uint32_t);
			case ImageFormat::RG32F: return 2 * sizeof(uint32_t);
			case ImageFormat::RGB8: return 3 * sizeof(uint8_t);
			case ImageFormat::RGB8I: return 3 * sizeof(uint8_t);
			case ImageFormat::RGB8U: return 3 * sizeof(uint8_t);
			case ImageFormat::RGB16: return 3 * sizeof(uint16_t);
			case ImageFormat::RGB16I: return 3 * sizeof(uint16_t);
			case ImageFormat::RGB16U: return 3 * sizeof(uint16_t);
			case ImageFormat::RGB16F: return 3 * sizeof(uint16_t);
			case ImageFormat::RGB32I: return 3 * sizeof(uint32_t);
			case ImageFormat::RGB32U: return 3 * sizeof(uint32_t);
			case ImageFormat::RGB32F: return 3 * sizeof(uint32_t);
			case ImageFormat::RGBA8: return 4 * sizeof(uint8_t);
			case ImageFormat::RGBA8I: return 4 * sizeof(uint8_t);
			case ImageFormat::RGBA8U: return 4 * sizeof(uint8_t);
			case ImageFormat::RGBA16: return 4 * sizeof(uint16_t);
			case ImageFormat::RGBA16I: return 4 * sizeof(uint16_t);
			case ImageFormat::RGBA16U: return 4 * sizeof(uint16_t);
			case ImageFormat::RGBA16F: return 4 * sizeof(uint16_t);
			case ImageFormat::RGBA32I: return 4 * sizeof(uint32_t);
			case ImageFormat::RGBA32U: return 4 * sizeof(uint32_t); 
			case ImageFormat::RGBA32F: return 4 * sizeof(uint32_t);
			case ImageFormat::D16: return sizeof(uint16_t);
			case ImageFormat::D24: return 3;
			case ImageFormat::D32: return sizeof(uint32_t);
			case ImageFormat::S8: return sizeof(uint8_t);
			case ImageFormat::S16: return sizeof(uint16_t);
			case ImageFormat::D24S8: return sizeof(uint32_t);
			case ImageFormat::D32FS8: return sizeof(uint32_t);
			default: return 0;
		}
	}

	void AllocTextureStorage(GLuint texture, ImageType type, uint32_t levels, uint32_t samples, ImageFormat format, uint32_t width, uint32_t height, uint32_t depth, bool isMutable) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		const GLenum openglFormat = ToOpenGLInternalImageFormat(format);
		
		if (samples != 1) {
			switch (type) {
				case ImageType::Texture2D:
					glTextureStorage2DMultisample(texture, samples, openglFormat, width, height, GL_TRUE);
					break;
				case ImageType::Texture2DArray:
					glTextureStorage3DMultisample(texture, samples, openglFormat, width, height, depth, GL_TRUE);
					break;
			}
		} else {
			switch (type) {
				case ImageType::Texture1D:
					glTextureStorage1D(texture, levels, openglFormat, width);
					break;
				case ImageType::Texture1DArray:
				case ImageType::Texture2D:
					glTextureStorage2D(texture, levels, openglFormat, width, height);
					break;
				case ImageType::Texture2DArray:
				case ImageType::Texture3D:
					glTextureStorage3D(texture, levels, openglFormat, width, height, depth);
					break;
			}
		}
	}

	OpenGLTexture::OpenGLTexture(const Ref<OpenGLGraphicsContext> &context, const TextureInfo &info) : mContext(context), mType(info.Type), mFormat(info.Format) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		AR_ASSERT(info.Width != 0, "Texture width cannot be 0");
		AR_ASSERT(info.Height != 0, "Texture height cannot be 0");
		AR_ASSERT(info.Levels != 0, "Texture levels cannot be 0");
		AR_ASSERT(info.Samples != 0, "Texture samples cannot be 0");

		GLuint maxSamples = 0;
		glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, (GLint*)&maxSamples);
		AR_ASSERT(info.Samples <= maxSamples, "Texture samples exceeds maximum samples (%u > %u)", info.Samples, maxSamples);

		glCreateTextures(ToOpenGLImageType(info.Type, info.Samples), 1, &mTexture);

		mWidth = info.Width;
		mHeight = info.Height;
		mDepth = info.Depth;
		mLevels = info.Levels;
		mSamples = info.Samples;

		AllocTextureStorage(mTexture, mType, mLevels, mSamples, mFormat, mWidth, mHeight, mDepth, true);
	}

	OpenGLTexture::~OpenGLTexture() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glDeleteTextures(1, &mTexture);
	}

	void OpenGLTexture::SetImage(uint32_t level, const Image &image) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		const GLenum texelType = GetOpenGLTexelType(image.Format);

		switch (mType) {
			case ImageType::Texture1D:
				glTextureSubImage1D(mTexture, level, 0, image.Width, ToOpenGLImageFormat(image.Format), texelType, image.Data);
				break;
			case ImageType::Texture2D:
				glTextureSubImage2D(mTexture, level, 0, 0, image.Width, image.Height, ToOpenGLImageFormat(image.Format), texelType, image.Data);
				break;
			case ImageType::Texture3D:
				glTextureSubImage3D(mTexture, level, 0, 0, 0, image.Width, image.Height, image.Depth, ToOpenGLImageFormat(image.Format), texelType, image.Data);
				break;
		}
	}

	void OpenGLTexture::Resize(uint32_t width, uint32_t height, uint32_t depth) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		if (width == mWidth && height == mHeight && depth == mDepth) return;
		
		mWidth = width;
		mHeight = height;
		mDepth = depth;

		glDeleteTextures(1, &mTexture);

		glCreateTextures(ToOpenGLImageType(mType, mSamples), 1, &mTexture);

		AllocTextureStorage(mTexture, mType, mLevels, mSamples, mFormat, mWidth, mHeight, mDepth, true);
	}

	GLenum ToOpenGLSamplerFilterWithMipmapFilter(SamplerFilter filter, SamplerFilter mipmapFilter) {
		if (filter == SamplerFilter::Nearest) {
			switch (mipmapFilter) {
				case SamplerFilter::Nearest: return GL_NEAREST_MIPMAP_NEAREST;
				case SamplerFilter::Linear: return GL_NEAREST_MIPMAP_LINEAR;
				default: return GL_NONE;
			}
		} else if (filter == SamplerFilter::Linear) {
			switch (mipmapFilter) {
				case SamplerFilter::Nearest: return GL_LINEAR_MIPMAP_NEAREST;
				case SamplerFilter::Linear: return GL_LINEAR_MIPMAP_LINEAR;
				default: return GL_NONE;
			}
		}
		return GL_NONE;
	}

	GLenum ToOpenGLSamplerFilter(SamplerFilter filter) {
		switch (filter) {
			case SamplerFilter::Nearest: return GL_NEAREST;
			case SamplerFilter::Linear: return GL_LINEAR;
			default: return GL_NONE;
		}
	}

	GLenum ToOpenGLSamplerWrap(SamplerWrap wrap) {
		switch (wrap) {
			case SamplerWrap::Repeat: return GL_REPEAT;
			case SamplerWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
			case SamplerWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
			case SamplerWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
			default: return GL_NONE;
		}
	}

	OpenGLSampler::OpenGLSampler(const Ref<OpenGLGraphicsContext> &context, const SamplerInfo &info) : mContext(context) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glCreateSamplers(1, &mSampler);
		glSamplerParameteri(mSampler, GL_TEXTURE_MIN_FILTER, ToOpenGLSamplerFilterWithMipmapFilter(info.MinFilter, info.MipmapFilter));
		glSamplerParameteri(mSampler, GL_TEXTURE_MAG_FILTER, ToOpenGLSamplerFilter(info.MagFilter));
		glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_S, ToOpenGLSamplerWrap(info.WrapS));
		glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_T, ToOpenGLSamplerWrap(info.WrapT));
		glSamplerParameteri(mSampler, GL_TEXTURE_WRAP_R, ToOpenGLSamplerWrap(info.WrapR));
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