#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Math/Vector3.hpp>

namespace Arcane {

	struct AABB {
	public:
		AABB(const Vector3 &position, const Vector3 &size) : Position(position), Size(size) { }
		AABB(const Vector3 &position, float size) : Position(position), Size(size) { }
		AABB() : Size(1.0f), Position(0.0f) { }

	public:
		Vector3 Position;
		Vector3 Size;
	};

}