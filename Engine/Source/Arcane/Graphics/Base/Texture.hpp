#pragma once

#include "GraphicsContext.hpp"
#include <Arcane/Native/NativeTexture.hpp>
#include <Arcane/Graphics/Loader.hpp>

namespace Arcane {

	class Texture {
	public:
		static Texture Create(const GraphicsContext &context, const TextureInfo &info);
		static Texture Create(const GraphicsContext &context, const ImageData &image);
	
	public:
		Texture() { }
		Texture(const Ref<NativeTexture> &nativeTexture) : mNativeTexture(nativeTexture) { }
		~Texture() { }

		inline void SetImage(uint32_t level, uint32_t index, const ImageData &image) { GetNativeTexture()->SetImage(level, index, image); }
		inline void GenerateMipmaps() { GetNativeTexture()->GenerateMipmaps(); }
		
		inline uint32_t GetWidth() const { return GetNativeTexture()->GetWidth(); }
		inline uint32_t GetHeight() const { return GetNativeTexture()->GetHeight(); }
		inline uint32_t GetDepth() const { return GetNativeTexture()->GetDepth(); }
		inline ImageFormat GetFormat() const { return GetNativeTexture()->GetFormat(); }
		inline uint32_t GetLevels() const { return GetNativeTexture()->GetLevels(); }
		inline uint32_t GetLayers() const { return GetNativeTexture()->GetLayers(); }
		inline uint32_t GetSampleCount() const { return GetNativeTexture()->GetSampleCount(); }
		inline bool HasFixedSampleLocations() const { return GetNativeTexture()->HasFixedSampleLocations(); }

		inline Ref<NativeTexture> GetNativeTexture() const {
			AR_ASSERT(mNativeTexture, "Native texture is invalid");
			return mNativeTexture;
		}

	private:
		Ref<NativeTexture> mNativeTexture;
	};

	class Sampler {
	public:
		static Sampler Create(const GraphicsContext &context, const SamplerInfo &info);

	public:
		Sampler() { }
		Sampler(const Ref<NativeSampler> &nativeSampler) : mNativeSampler(nativeSampler) { }
		~Sampler() { }

		inline SamplerFilter GetMinFilter() const { return GetNativeSampler()->GetMinFilter(); }
		inline SamplerFilter GetMagFilter() const { return GetNativeSampler()->GetMagFilter(); }
		inline SamplerFilter GetMipmapFilter() const { return GetNativeSampler()->GetMipmapFilter(); }
		inline float GetMinLOD() const { return GetNativeSampler()->GetMinLOD(); }
		inline float GetMaxLOD() const { return GetNativeSampler()->GetMaxLOD(); }
		inline Color GetBorderColor() const { return GetNativeSampler()->GetBorderColor(); }
		inline SamplerWrap GetWrapS() const { return GetNativeSampler()->GetWrapS(); }
		inline SamplerWrap GetWrapT() const { return GetNativeSampler()->GetWrapT(); }
		inline SamplerWrap GetWrapR() const { return GetNativeSampler()->GetWrapR(); }

		inline Ref<NativeSampler> GetNativeSampler() const {
			AR_ASSERT(mNativeSampler, "Native sampler is invalid");
			return mNativeSampler;
		}

	private:
		Ref<NativeSampler> mNativeSampler;
	};
	
}