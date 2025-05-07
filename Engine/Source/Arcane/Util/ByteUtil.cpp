#include "ByteUtil.hpp"

namespace Arcane {

	uint16_t EndianSwap(uint16_t n) {
		return __builtin_bswap16(n);
	}

	uint32_t EndianSwap(uint32_t n) {
		return __builtin_bswap32(n);
	}

	uint64_t EndianSwap(uint64_t n) {
		return __builtin_bswap64(n);
	}

}