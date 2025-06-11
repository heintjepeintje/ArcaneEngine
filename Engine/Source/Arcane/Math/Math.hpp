#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	static constexpr float Pi = 3.14159265358979323846f;
	static constexpr float HalfPi = Pi / 2.0f;
	static constexpr float TwoPi = Pi * 2.0f;

	constexpr float Floor(float n) {
		return (float)(int32_t)n;
	}

	constexpr float Ceil(float n) {
		return Floor(n) + 1.0f;
	}

	constexpr float Round(float n) {
		return (Floor(n) + 0.5f > n) ? Floor(n) : Ceil(n); 
	}

	template<typename _Type>
	constexpr _Type Min(const _Type &a, const _Type &b) {
		return a < b ? a : b;
	}

	template<typename _Type>
	constexpr _Type Max(const _Type &a, const _Type &b) {
		return a > b ? a : b;
	}

	template<typename _Type>
	constexpr _Type Clamp(const _Type &value, const _Type &min, const _Type &max) {
		return Min(Max(value, min), max);
	}

	template<typename _Type>
	constexpr _Type Normalize(const _Type &value, const _Type &min, const _Type &max) {
		return (value - min) / (max - min);
	}

	template<typename _Type>
	constexpr _Type Lerp(const _Type &value, const _Type &min, const _Type &max) {
		return value * (max - min) + min;
	}

	template<typename _Type>
	constexpr _Type Map(const _Type value, const _Type srcMin, const _Type srcMax, const _Type dstMin, const _Type dstMax) {
		return Lerp(Normalize(value, srcMin, srcMax), dstMin, dstMax);
	}

	constexpr float Abs(float value) {
		return value < 0.0f ? -value : value;
	}

	float Sin(float angle);
	float Cos(float angle);
	float Tan(float angle);
	float Sqrt(float value);

	constexpr float ToRadians(float angle) {
		return (angle / 180.0f) * Pi;
	}

	constexpr float ToDegrees(float angle) {
		return (angle / Pi) * 180.0f;
	}

	enum class RandomGenerator {
		None = 0,
		LFSR, LCG
	};

	void SetRandomGenerator(RandomGenerator generator);
	void SetRandomState(uint64_t state);
	void IterateRandom(uint32_t count);
	bool RandomBool();
	uint8_t RandomUInt8();
	uint16_t RandomUInt16();
	uint32_t RandomUInt32();
	uint64_t RandomUInt64();
	int8_t RandomInt8();
	int16_t RandomInt16();
	int32_t RandomInt32();
	int64_t RandomInt64();
	float RandomFloat();
	double RandomDouble();

	int8_t RandomRangeInt8(int8_t min, int8_t max);
	int16_t RandomRangeInt16(int16_t min, int16_t max);
	int32_t RandomRangeInt32(int32_t min, int32_t max);
	int64_t RandomRangeInt64(int64_t min, int64_t max);
	float RandomRangeFloat(int8_t min, int8_t max);
	double RandomRangeDouble(double min, double max);

}