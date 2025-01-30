#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/Image.hpp>
#include "NativeGraphicsContext.hpp"

namespace Arcane {

	struct TextureInfo {
		ImageType Type;
		ImageFormat Format;
		uint32_t Levels;
		uint32_t Width, Height;
		uint32_t Depth;
		uint32_t Samples;
	};

	class NativeTexture {
	public:
		static Ref<NativeTexture> Create(const Ref<NativeGraphicsContext> &context, const TextureInfo &info);
	
	public:
		NativeTexture() { }
		virtual ~NativeTexture() { }

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void SetImage(uint32_t level, const Image &image) = 0;

		virtual ImageFormat GetFormat() = 0;
		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;
		virtual uint32_t GetDepth() = 0;
		virtual uint32_t GetLevels() = 0;
		virtual ImageType GetType() = 0;
	};

	enum class SamplerFilter {
		None = 0,
		Nearest,
		Linear
	};

	enum class SamplerWrap {
		None = 0,
		Repeat,
		MirroredRepeat,
		ClampToEdge,
		ClampToBorder
	};

	struct SamplerInfo {
		SamplerFilter MinFilter;
		SamplerFilter MagFilter;
		SamplerWrap WrapS;
		SamplerWrap WrapT;
		SamplerWrap WrapR;
	};

	class NativeSampler {
	public:
		static Ref<NativeSampler> Create(const Ref<NativeGraphicsContext> &context, const SamplerInfo &info);

	public:
		NativeSampler() { }
		virtual ~NativeSampler() { }
	};

}