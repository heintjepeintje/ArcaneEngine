#include "Loader.hpp"

#include <Arcane/Util/ByteUtil.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Arcane {

	ImageFormat ChannelsToImageFormat(uint32_t channels) {
		switch (channels) {
			case 3: return ImageFormat::RGB8;
			case 4: return ImageFormat::RGBA8;
		}

		return ImageFormat::None;
	}

	uint32_t ImageFormatToChannels(ImageFormat format) {
		switch (format) {
			case ImageFormat::RGB8: return 3;
			case ImageFormat::RGBA8: return 4;
		}

		return 0;
	}

	ImageData LoadImage(const std::string &path, ImageFormat requestedFormat) {
		int width = 0;
		int height = 0;
		int channels = 0;

		const uint32_t requestedChannels = ImageFormatToChannels(requestedFormat);

		FILE *f = fopen(path.c_str(), "rb");
		AR_ASSERT(f, "Failed to open image: {}\n", path);

		const stbi_uc *pixels = stbi_load_from_file(f, &width, &height, &channels, requestedChannels);
		fclose(f);

		AR_ASSERT(pixels, "Failed to load image: {}\n\t{}\n", path, stbi_failure_reason());

		ImageData data;
		data.Width = width;
		data.Height = height;

		if (requestedFormat == ImageFormat::None) {
			data.Format = ChannelsToImageFormat(channels);
		} else {
			data.Format = requestedFormat;
		}

		uint32_t outputChannels = ImageFormatToChannels(data.Format);

		data.Data = AllocateBuffer(width * height * outputChannels);
		memcpy(data.Data.GetPointer(), pixels, width * height * outputChannels);
		stbi_image_free((void*)pixels);

		return data;
	}

	ImageData LoadImage(const Color &color, ImageFormat format) {
		ImageData output{};

		if (format == ImageFormat::RGBA8) {
			output.Width = 1;
			output.Height = 1;
			output.Format = ImageFormat::RGBA8;
			
			const uint8_t colorBytes[4] = {
				(uint8_t)Round(Clamp(color.R, 0.0f, 1.0f) * 255.0f),
				(uint8_t)Round(Clamp(color.G, 0.0f, 1.0f) * 255.0f),
				(uint8_t)Round(Clamp(color.B, 0.0f, 1.0f) * 255.0f),
				(uint8_t)Round(Clamp(color.A, 0.0f, 1.0f) * 255.0f),
			};

			output.Data = AllocateBuffer(4 * sizeof(uint8_t));
			std::memcpy(output.Data.GetPointer(), colorBytes, 4 * sizeof(uint8_t));
		} else if (format == ImageFormat::RGB8) {
			output.Width = 1;
			output.Height = 1;
			output.Format = ImageFormat::RGB8;
			
			const uint8_t colorBytes[3] = {
				(uint8_t)Round(Clamp(color.R, 0.0f, 1.0f) * 255.0f),
				(uint8_t)Round(Clamp(color.G, 0.0f, 1.0f) * 255.0f),
				(uint8_t)Round(Clamp(color.B, 0.0f, 1.0f) * 255.0f),
			};

			output.Data = AllocateBuffer(3 * sizeof(uint8_t));
			std::memcpy(output.Data.GetPointer(), colorBytes, 3 * sizeof(uint8_t));
		}

		return output;
	}

	static void ImageProcessFlipHorizontal(ImageData &data) {
		const uint32_t pixelSize = data.Format == ImageFormat::RGBA8 ? 4 : 3;

		if (data.Format == ImageFormat::RGBA8 || data.Format == ImageFormat::RGB8) {
			void *flipped = alloca(data.Width * data.Height * pixelSize);
			memset(flipped, 0xFFFFFFFF, data.Width * data.Height * pixelSize);

			for (uint32_t y = 0; y < data.Height; y++) {
				for (uint32_t x = 0; x < data.Width; x++) {
					memcpy(
						AR_PTR_ADD(flipped, (x + y * data.Width)),
						AR_PTR_ADD(data.Data, (data.Width - x - 1) + y * data.Width),
						pixelSize
					);
				}
			}

			memcpy(data.Data.GetPointer(), flipped, data.Width * data.Height * pixelSize);
			return;
		}
	}

	static void ImageProcessFlipVertical(ImageData &data) {
		const uint32_t pixelSize = data.Format == ImageFormat::RGBA8 ? 4 : 3;
		
		if (data.Format == ImageFormat::RGBA8 || data.Format == ImageFormat::RGB8) {
			uint8_t *flipped = new uint8_t[data.Width * data.Height * pixelSize];

			for (uint32_t y = 0; y < data.Height; ++y) {
				uint32_t srcOffset = y * data.Width * pixelSize;
				uint32_t dstOffset = (data.Height - 1 - y) * data.Width * pixelSize;

				memcpy(
					AR_PTR_ADD(flipped, dstOffset), 
					AR_PTR_ADD(data.Data, srcOffset), 
					data.Width * pixelSize
				);
			}

			memcpy(data.Data.GetPointer(), flipped, data.Width * data.Height * pixelSize);
			delete[] flipped;
		}
	}

	static void ImageProcessInvert(ImageData &data) {
		uint32_t *rawData = data.Data.GetPointerAs<uint32_t>();
		if (data.Format == ImageFormat::RGBA8) {
			for (uint32_t i = 0; i < data.Width * data.Height; i++) {
				uint8_t alpha = (rawData[i] & 0x000000FF);
				rawData[i] = ~rawData[i] & (0xFFFFFF00 | (uint32_t)alpha);
			}
		} else if (data.Format == ImageFormat::RGB8) {
			for (uint32_t i = 0; i < data.Width * data.Height; i++) {
				rawData[i] = ~rawData[i];
			}
		}
	}

	void ProcessImage(ImageData &data, ImageProcess process) {
		switch (process) {
			case ImageProcess::FlipVertical:
				ImageProcessFlipVertical(data);
				break;
		}
	}

}