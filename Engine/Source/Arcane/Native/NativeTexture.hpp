#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/Color.hpp>
#include <Arcane/Graphics/ImageData.hpp>
#include "NativeGraphicsContext.hpp"

namespace Arcane {

	enum class TextureType {
		None = 0,
		Texture1D,
		Texture2D,
		Texture3D,
		CubeMap
	};

	struct TextureInfo {
		TextureType Type;
		ImageFormat Format;
		uint32_t Levels;
		uint32_t Layers;
		uint32_t Width, Height, Depth;
		uint32_t Samples;
		bool FixedSampleLocations;
	};

	class NativeTexture {
	public:
		static Ref<NativeTexture> Create(const Ref<NativeGraphicsContext> &context, const TextureInfo &info);
	
	public:
		NativeTexture() { }
		virtual ~NativeTexture() { }
		
		virtual void GenerateMipmaps() = 0;
		virtual void SetImage(uint32_t level, uint32_t index, const ImageData &image) = 0;

		virtual ImageFormat GetFormat() const = 0;
		virtual TextureType GetType() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetDepth() const = 0;
		virtual uint32_t GetLevels() const = 0;
		virtual uint32_t GetLayers() const = 0;
		virtual uint32_t GetSampleCount() const = 0;
		virtual bool HasFixedSampleLocations() const = 0;

		virtual void Destroy() = 0;
		virtual bool IsValid() const = 0;
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
		SamplerFilter MipmapFilter;
		float MinLOD;
		float MaxLOD;
		Color BorderColor;
		SamplerWrap WrapS;
		SamplerWrap WrapT;
		SamplerWrap WrapR;
	};

	class NativeSampler {
	public:
		static Ref<NativeSampler> Create(const Ref<NativeGraphicsContext> &context, const SamplerInfo &info);

		virtual SamplerFilter GetMinFilter() const = 0;
		virtual SamplerFilter GetMagFilter() const = 0;
		virtual SamplerFilter GetMipmapFilter() const = 0;
		virtual float GetMinLOD() const = 0;
		virtual float GetMaxLOD() const = 0;
		virtual Color GetBorderColor() const = 0;
		virtual SamplerWrap GetWrapS() const = 0;
		virtual SamplerWrap GetWrapT() const = 0;
		virtual SamplerWrap GetWrapR() const = 0;

	public:
		NativeSampler() { }
		virtual ~NativeSampler() { }
	};

}