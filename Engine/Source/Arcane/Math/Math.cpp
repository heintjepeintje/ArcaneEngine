#include "Math.hpp"

#include <cmath>

#define AR_LCG_MODULUS 2147483648
#define AR_LCG_MULTIPLIER 1103515245
#define AR_LCG_INCREMENT 12345

namespace Arcane {

	float Sin(float angle) {
		return sinf(angle);
	}

	float Cos(float angle) {
		return cosf(angle);
	}

	float Tan(float angle) {
		return tanf(angle);
	}

	float Sqrt(float value) {
		return sqrtf(value);
	}

	uint64_t sState = 1013904223;
	RandomGenerator sGenerator = RandomGenerator::LCG;

	void LFSRXorShift(uint32_t count) {
		for (uint32_t i = 0; i < count; i++) {
			uint8_t bit = (
				(sState >> 24) ^ 
				(sState >> 23) ^ 
				(sState >> 22) ^ 
				(sState >> 17)
			) & 1u;

			sState = (sState >> 1) | (((uint64_t)bit) << 63);
		}
	}

	void LCG() {
		uint64_t rhs = (AR_LCG_MULTIPLIER * sState + AR_LCG_INCREMENT) % AR_LCG_MODULUS;
		uint64_t lhs = (AR_LCG_MULTIPLIER * sState + AR_LCG_INCREMENT) % AR_LCG_MODULUS;
		sState = rhs | (lhs << 32);
	}

	uint64_t NewRandom(uint32_t bits) {
		switch (sGenerator) {
			case RandomGenerator::LFSR: LFSRXorShift(bits); break; 
			case RandomGenerator::LCG: LCG(); break;
		}

		return sState >> (64 - bits);
	}

	void SetRandomGenerator(RandomGenerator generator) {
		sGenerator = generator;
	} 

	void SetRandomState(uint64_t state) {
		sState = state;
	}

	void IterateRandom(uint32_t count) {
		LFSRXorShift(count);
	}

	bool RandomBool() {
		return NewRandom(1);
	}

	uint8_t RandomUInt8() {
		return NewRandom(8);
	}

	uint16_t RandomUInt16() {
		return NewRandom(16);
	}

	uint32_t RandomUInt32() {
		return NewRandom(32);
	}

	uint64_t RandomUInt64() {
		return NewRandom(64);
	}

	int8_t RandomInt8() {
		uint8_t result = NewRandom(8);
		return *(int8_t*)&result;
	}

	int16_t RandomInt16() {
		uint16_t result = NewRandom(16);
		return *(int16_t*)&result;
	}

	int32_t RandomInt32() {
		uint32_t result = NewRandom(32);
		return *(int32_t*)&result;
	}

	int64_t RandomInt64() {
		uint64_t result = NewRandom(64);
		return *(int64_t*)&result;
	}

	float RandomFloat() {
		uint32_t result = NewRandom(23);
		return *(float*)&result;
	}

	double RandomDouble() {
		uint64_t result = NewRandom(52);
		return *(float*)&result;
	}

	int8_t RandomRangeInt8(int8_t min, int8_t max) {
		int8_t r = RandomInt8();
		return Map<int8_t>(r, INT8_MIN, INT8_MAX, min, max);
	}

	int16_t RandomRangeInt16(int16_t min, int16_t max) {
		int16_t r = RandomInt16();
		return Map<int16_t>(r, INT16_MIN, INT16_MAX, min, max);
	}

	int32_t RandomRangeInt32(int32_t min, int32_t max) {
		int32_t r = RandomInt16();
		return Map<int32_t>(r, INT32_MIN, INT32_MAX, min, max);
	}

	int64_t RandomRangeInt64(int64_t min, int64_t max) {
		int64_t r = RandomInt16();
		return Map<int64_t>(r, INT64_MIN, INT64_MAX, min, max);
	}

	float RandomRangeFloat(float min, float max) {
		float r = RandomFloat();
		return Lerp(r, min, max);
	}

	double RandomRangeDouble(double min, double max) {
		double r = RandomDouble();
		return Lerp(r, min, max);
	}

}