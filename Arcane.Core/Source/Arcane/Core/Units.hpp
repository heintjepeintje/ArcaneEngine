#pragma once

#include "./Types.hpp"

namespace Arcane {

	inline constexpr U64 from_kilobytes(F64 n) { return 1'000.0f * n; }
	inline constexpr U64 from_megabytes(F64 n) { return 1'000'000.0f * n; }
	inline constexpr U64 from_gigabytes(F64 n) { return 1'000'000'000.0f * n; }

	inline constexpr F64 to_kilobytes(U64 n) { return static_cast<F64>(n) / 1'000.0f; }
	inline constexpr F64 to_megabytes(U64 n) { return static_cast<F64>(n) / 1'000'000.0f; }
	inline constexpr F64 to_gigabytes(U64 n) { return static_cast<F64>(n) / 1'000'000'000.0f; }

	inline constexpr U64 from_kibibytes(F64 n) { return 1'024.0f * n; }
	inline constexpr U64 from_mebibytes(F64 n) { return 1'048'576.0f * n; }
	inline constexpr U64 from_gibibytes(F64 n) { return 1'098'907'648.0f * n; }

	inline constexpr F64 to_kibibytes(U64 n) { return static_cast<F64>(n) / 1'024.0f; }
	inline constexpr F64 to_mebibytes(U64 n) { return static_cast<F64>(n) / 1'048'576.0f; }
	inline constexpr F64 to_gibibytes(U64 n) { return static_cast<F64>(n) / 1'098'907'648.0f; }

}