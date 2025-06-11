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

	bool IsDepthFormat(ImageFormat format);
	bool IsStencilFormat(ImageFormat format);
	bool IsColorFormat(ImageFormat format);
	size_t GetTexelSize(ImageFormat format);

	struct ImageData {
		ImageFormat Format;
		uint32_t Width, Height, Depth;
		void *Data;
	};

}