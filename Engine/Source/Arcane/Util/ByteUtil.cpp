#include "ByteUtil.hpp"

namespace Arcane {

	uint16_t EndianSwap(uint16_t n) {
		return (n >> 8) | (n << 8);
	}

	uint32_t EndianSwap(uint32_t n) {
		return (n >> 24) | ((n >> 8) & 0xFF) | ((n << 8) & 0xFF) | (n << 24);
	}

	uint64_t EndianSwap(uint64_t n) {
		return 
			(n >> 56) | 
			((n >> 40) & 0xFF) | 
			((n >> 24) & 0xFF) | 
			((n >> 8 ) & 0xFF) | 
			((n << 8 ) & 0xFF) | 
			((n << 24) & 0xFF) | 
			((n << 40) & 0xFF) | 
			(n << 56);
	}

}