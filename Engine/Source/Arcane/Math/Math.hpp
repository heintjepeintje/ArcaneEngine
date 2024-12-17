#pragma once

namespace Arcane {

	static constexpr float Pi = 3.14159265358979323846f;
	static constexpr float HalfPi = Pi / 2.0f;
	static constexpr float TwoPi = Pi * 2.0f;

	constexpr float Min(float a, float b) {
		return a < b ? a : b;
	}

	constexpr float Max(float a, float b) {
		return a > b ? a : b;
	}

	constexpr float Clamp(float value, float min, float max) {
		return Min(max, Max(value, min));
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

}