#pragma once

#include "Vector.hpp"

namespace Arcane {

	struct Rect2D {
		Rect2D(const Vector2 &position, const Vector2 &size) : Position(position), Size(size) { }
		Rect2D() : Position(0.0f), Size(0.0f) { }
		~Rect2D() { }

		Vector2 Position;
		Vector2 Size;
	};

	constexpr float Min(float a, float b) {
		return a < b ? a : b;
	}

	constexpr float Max(float a, float b) {
		return a > b ? a : b;
	}

	constexpr float Clamp(float value, float min, float max) {
		return Min(max, Max(value, min));
	}

}