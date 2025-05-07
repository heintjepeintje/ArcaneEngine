#pragma once

#include "Math.hpp"
#include "Vector3.hpp"

namespace Arcane {

	class Quaternion {
	public:
		Quaternion(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
		Quaternion() : X(0), Y(0), Z(0), W(1) { }
		~Quaternion() { }

		inline Vector3 ToEuler() const {
			Vector3 rotation;
			rotation.X = std::atan2(
				2 * (W * X + Y * Z),
				1 - 2 * (X * X + Y * Y)
			);

			rotation.Y = std::atan2(
				std::sqrt(1 + 2 * (W * Y + X * Z)),
				std::sqrt(1 - 2 * (W * Y + X * Z))
			);

			rotation.Z = std::atan2(
				2 * (W * Z + X * Y),
				1 - 2 * (Y * Y + Z * Z)
			);

			return rotation;
		}

	public:
		float X, Y, Z, W;
	};

}