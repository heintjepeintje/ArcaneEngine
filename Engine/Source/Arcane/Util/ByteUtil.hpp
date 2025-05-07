#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	uint16_t EndianSwap(uint16_t n);
	uint32_t EndianSwap(uint32_t n);
	uint64_t EndianSwap(uint64_t n);

	template<typename _Type>
	_Type EndianSwap(_Type v) {
		switch (sizeof(_Type)) {
			case 2: {
				uint16_t n = EndianSwap(*(uint16_t*)&v);
				return *(_Type*)&n;
			}
			case 4: {
				uint32_t n = EndianSwap(*(uint32_t*)&v);
				return *(_Type*)&n;
			}
			case 8: {
				uint64_t n = EndianSwap(*(uint64_t*)&v);
				return *(_Type*)&n;
			}
		}
	}
	
	enum class Endianness {
		None = 0,
		LittleEndian, BigEndian
	};

	template<Endianness _Endianness, typename _Type>
	inline _Type ToNativeEndian(_Type v) {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		if constexpr (_Endianness == Endianness::LittleEndian) {
			return v;
		} else if constexpr (_Endianness == Endianness::BigEndian) {
			return EndianSwap(v);
		}
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		if constexpr (_Endianness == Endianness::LittleEndian) {
			return EndianSwap(v);
		} else if constexpr (_Endianness == Endianness::BigEndian) {
			return v;
		}
#endif
	}

}