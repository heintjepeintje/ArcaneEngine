#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	enum class ImageFormat {
		None = 0,
		R8, R8I, R8U,
		R16, R16I, R16U, R16F,
		R32I, R32U, R32F,
		RG8, RG8I, RG8U,
		RG16, RG16I, RG16U, RG16F,
		RG32I, RG32U, RG32F,
		RGB8, RGB8I, RGB8U,
		RGB16, RGB16I, RGB16U, RGB16F,
		RGB32I, RGB32U, RGB32F,
		RGBA8, RGBA8I, RGBA8U,
		RGBA16, RGBA16I, RGBA16U, RGBA16F,
		RGBA32I, RGBA32U, RGBA32F,
		D16, D24, D32, D32F,
		S1, S4, S8, S16, 
		D24S8, D32FS8
	};

	inline bool IsDepthFormat(ImageFormat format) {
		switch (format) {
			case ImageFormat::D16: return true;
			case ImageFormat::D24: return true;
			case ImageFormat::D24S8: return true;
			case ImageFormat::D32: return true;
			case ImageFormat::D32F: return true;
			case ImageFormat::D32FS8: return true;
			default: return false;
		}
	}

	inline bool IsStencilFormat(ImageFormat format) {
		switch (format) {
			case ImageFormat::S1: return true;
			case ImageFormat::S4: return true;
			case ImageFormat::S8: return true;
			case ImageFormat::S16: return true;
			case ImageFormat::D24S8: return true;
			case ImageFormat::D32FS8: return true;
			default: return false;
		}
	}

	inline bool IsColorFormat(ImageFormat format) {
		return !IsDepthFormat(format) && !IsStencilFormat(format);
	}

	struct ImageData {
		ImageFormat Format;
		uint32_t Width, Height, Depth;
		void *Data;
	};

}