#pragma once

#include <Arcane/Core.hpp>
#include "Vector2.hpp"

namespace Arcane {

	struct Rect2D {
		Rect2D(const Vector2 &position, const Vector2 &size) : Position(position), Size(size) { }
		Rect2D(float x, float y, float width, float height) : Position(x, y), Size(width, height) { }
		Rect2D(const Vector2 &size) : Position(0.0f), Size(size) { }
		Rect2D(float width, float height) : Position(0.0f), Size(width, height) { }
		Rect2D() : Position(0.0f), Size(0.0f) { }
		~Rect2D() { }

		Vector2 Position;
		Vector2 Size;
	};

}
