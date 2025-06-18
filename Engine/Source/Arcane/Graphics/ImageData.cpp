#include "ImageData.hpp"

namespace Arcane {

	bool IsDepthFormat(ImageFormat format) {
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

	bool IsStencilFormat(ImageFormat format) {
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

	bool IsColorFormat(ImageFormat format) {
		return !IsDepthFormat(format) && !IsStencilFormat(format);
	}

	size_t GetChannelSize(ImageFormat format) {
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
			case ImageFormat::RG8: return sizeof(uint8_t);
			case ImageFormat::RG8I: return sizeof(uint8_t);
			case ImageFormat::RG8U: return sizeof(uint8_t);
			case ImageFormat::RG16: return sizeof(uint16_t);
			case ImageFormat::RG16I: return sizeof(uint16_t);
			case ImageFormat::RG16U: return sizeof(uint16_t);
			case ImageFormat::RG16F: return sizeof(uint16_t);
			case ImageFormat::RG32I: return sizeof(uint32_t);
			case ImageFormat::RG32U: return sizeof(uint32_t);
			case ImageFormat::RG32F: return sizeof(uint32_t);
			case ImageFormat::RGB8: return sizeof(uint8_t);
			case ImageFormat::RGB8I: return sizeof(uint8_t);
			case ImageFormat::RGB8U: return sizeof(uint8_t);
			case ImageFormat::RGB16: return sizeof(uint16_t);
			case ImageFormat::RGB16I: return sizeof(uint16_t);
			case ImageFormat::RGB16U: return sizeof(uint16_t);
			case ImageFormat::RGB16F: return sizeof(uint16_t);
			case ImageFormat::RGB32I: return sizeof(uint32_t);
			case ImageFormat::RGB32U: return sizeof(uint32_t);
			case ImageFormat::RGB32F: return sizeof(uint32_t);
			case ImageFormat::RGBA8: return sizeof(uint8_t);
			case ImageFormat::RGBA8I: return sizeof(uint8_t);
			case ImageFormat::RGBA8U: return sizeof(uint8_t);
			case ImageFormat::RGBA16: return sizeof(uint16_t);
			case ImageFormat::RGBA16I: return sizeof(uint16_t);
			case ImageFormat::RGBA16U: return sizeof(uint16_t);
			case ImageFormat::RGBA16F: return sizeof(uint16_t);
			case ImageFormat::RGBA32I: return sizeof(uint32_t);
			case ImageFormat::RGBA32U: return sizeof(uint32_t); 
			case ImageFormat::RGBA32F: return sizeof(uint32_t);
			case ImageFormat::D16: return sizeof(uint16_t);
			case ImageFormat::D24: return 3;
			case ImageFormat::D32: return sizeof(uint32_t);
			case ImageFormat::S8: return sizeof(uint8_t);
			case ImageFormat::S16: return sizeof(uint16_t);
			default: return 0;
		}
	}

	uint32_t GetChannelCount(ImageFormat format) {
		switch (format) {
			case ImageFormat::R8: return 1;
			case ImageFormat::R8I: return 1;
			case ImageFormat::R8U: return 1;
			case ImageFormat::R16: return 1;
			case ImageFormat::R16I: return 1;
			case ImageFormat::R16U: return 1;
			case ImageFormat::R16F: return 1;
			case ImageFormat::R32I: return 1;
			case ImageFormat::R32U: return 1;
			case ImageFormat::R32F: return 1;
			case ImageFormat::RG8: return 2;
			case ImageFormat::RG8I: return 2;
			case ImageFormat::RG8U: return 2;
			case ImageFormat::RG16: return 2;
			case ImageFormat::RG16I: return 2;
			case ImageFormat::RG16U: return 2;
			case ImageFormat::RG16F: return 2;
			case ImageFormat::RG32I: return 2;
			case ImageFormat::RG32U: return 2;
			case ImageFormat::RG32F: return 2;
			case ImageFormat::RGB8: return 3;
			case ImageFormat::RGB8I: return 3;
			case ImageFormat::RGB8U: return 3;
			case ImageFormat::RGB16: return 3;
			case ImageFormat::RGB16I: return 3;
			case ImageFormat::RGB16U: return 3;
			case ImageFormat::RGB16F: return 3;
			case ImageFormat::RGB32I: return 3;
			case ImageFormat::RGB32U: return 3;
			case ImageFormat::RGB32F: return 3;
			case ImageFormat::RGBA8: return 4;
			case ImageFormat::RGBA8I: return 4;
			case ImageFormat::RGBA8U: return 4;
			case ImageFormat::RGBA16: return 4;
			case ImageFormat::RGBA16I: return 4;
			case ImageFormat::RGBA16U: return 4;
			case ImageFormat::RGBA16F: return 4;
			case ImageFormat::RGBA32I: return 4;
			case ImageFormat::RGBA32U: return 4; 
			case ImageFormat::RGBA32F: return 4;
			case ImageFormat::D16: return 1;
			case ImageFormat::D24: return 1;
			case ImageFormat::D32: return 1;
			case ImageFormat::S8: return 1;
			case ImageFormat::S16: return 1;
			default: return 0;
		}
	}

	size_t GetTexelSize(ImageFormat format) {
		return GetChannelSize(format) * GetChannelCount(format);
	}

}