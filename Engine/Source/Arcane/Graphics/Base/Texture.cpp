#include "Texture.hpp"

namespace Arcane {

	Texture Texture::Create(const GraphicsContext &context, const TextureInfo &info) {
		return Texture(NativeTexture::Create(
			context.GetNativeContext(), 
			info
		));
	}

	Texture Texture::Create(const GraphicsContext &context, const ImageData &data) {
		TextureInfo info{};
		info.Width = data.Width;
		info.Height = data.Height;
		info.Depth = 1;
		info.Levels = 1;
		info.Layers = 1;
		info.Samples = 1;
		info.FixedSampleLocations = true;
		info.Format = data.Format;
		info.Type = TextureType::Texture2D;

		Texture t = Texture::Create(context, info);

		t.SetImage(0, 0, data);

		return t;
	}

	Sampler Sampler::Create(const GraphicsContext &context, const SamplerInfo &info) {
		return Sampler(NativeSampler::Create(
			context.GetNativeContext(), 
			info
		));
	}

}