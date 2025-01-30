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
		info.Samples = 1;
		info.Format = data.Format;
		info.Type = ImageType::Texture2D;

		Texture t = Texture::Create(context, info);
		
		Image image{};
		image.Width = data.Width;
		image.Height = data.Height;
		image.Format = data.Format;
		image.Depth = 1;
		image.Data = data.Data;

		t.SetImage(0, image);

		return t;
	}

	Sampler Sampler::Create(const GraphicsContext &context, const SamplerInfo &info) {
		return Sampler(NativeSampler::Create(
			context.GetNativeContext(), 
			info
		));
	}

}