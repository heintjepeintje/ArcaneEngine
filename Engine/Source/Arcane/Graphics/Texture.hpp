#pragma once

#include "GraphicsContext.hpp"
#include <Arcane/Native/NativeTexture.hpp>
#include "Loader.hpp"

namespace Arcane {

	class Texture {
	public:
		static Texture Create(const GraphicsContext &context, const TextureInfo &info);
		static Texture Create(const GraphicsContext &context, const ImageData &image);
	
	public:
		Texture() { }
		Texture(const Ref<NativeTexture> &nativeTexture) : mNativeTexture(nativeTexture) { }
		~Texture() { }

		inline void SetImage(uint32_t index, const Image &image) { GetNativeTexture()->SetImage(index, image); }
		inline void GenerateMipmaps() { GetNativeTexture()->GenerateMipmaps(); }
		inline void Resize(uint32_t width, uint32_t height, uint32_t depth) { GetNativeTexture()->Resize(width, height, depth); }

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
		~Sampler() { }

		inline Ref<NativeSampler> GetNativeSampler() const {
			AR_ASSERT(mNativeSampler, "Native sampler is invalid");
			return mNativeSampler;
		}

	private:
		Sampler(const Ref<NativeSampler> &nativeSampler) : mNativeSampler(nativeSampler) { }

	private:
		Ref<NativeSampler> mNativeSampler;
	};
	
}