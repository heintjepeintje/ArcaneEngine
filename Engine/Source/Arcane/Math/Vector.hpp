#pragma once

#include "Math.hpp"

namespace Arcane {

	class Vector2 {
	public:
		Vector2(float x, float y) : x(x), y(y) { }
		Vector2(float xy) : x(xy), y(xy) { }
		Vector2() : x(0), y(0) { }
		Vector2(const Vector2 &other) : x(other.x), y(other.y) { }
		Vector2 &operator=(const Vector2 &other) {
			x = other.x;
			y = other.y;
			return *this;
		}

		~Vector2() { }

	public:
		union {
			struct {
				float x, y;
			};
			float data[2];
		};
	};

}