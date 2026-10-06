#pragma once

#include "./Platform.hpp"

namespace Arcane {

	using I8 = AR_INT8_TYPE;
	using I16 = AR_INT16_TYPE;
	using I32 = AR_INT32_TYPE;
	using I64 = AR_INT64_TYPE;

	using U8 = AR_UINT8_TYPE;
	using U16 = AR_UINT16_TYPE;
	using U32 = AR_UINT32_TYPE;
	using U64 = AR_UINT64_TYPE;

	using F32 = AR_FLOAT32_TYPE;
	using F64 = AR_FLOAT64_TYPE;

	using C8 = AR_CHAR8_TYPE;
	using C16 = AR_CHAR16_TYPE;

	using B8 = AR_UINT8_TYPE;

	template<typename T> class RemoveReference { public: using Type = T; };
	template<typename T> class RemoveReference<T&> { public: using Type = T; };
	template<typename T> class RemoveReference<T&&> { public: using Type = T; };

	template<typename T> class RemoveConst { public: using Type = T; };
	template<typename T> class RemoveConst<const T> { public: using Type = T; };

}
