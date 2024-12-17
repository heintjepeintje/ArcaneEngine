#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	class Vector2 {
	public:
		inline static Vector2 MaxValue() { return Vector2(__FLT_MAX__, __FLT_MAX__); }
		inline static Vector2 MinValue() { return Vector2(-__FLT_MAX__, -__FLT_MAX__); }
		inline static Vector2 Zero() { return Vector2(0.0f, 0.0f); }
		inline static Vector2 Left() { return Vector2(-1.0f, 0.0f); }
		inline static Vector2 Right() { return Vector2(1.0f, 0.0f); }
		inline static Vector2 Up() { return Vector2(0.0f, 1.0f); }
		inline static Vector2 Down() { return Vector2(0.0f, -1.0f); }

	public:
		Vector2(float x, float y) : x(x), y(y) { }
		Vector2(float xy) : x(xy), y(xy) { }
		Vector2() : x(0), y(0) { }
		~Vector2() { }

		inline Vector2 operator/(float scalar) {
			return Vector2(x / scalar, y / scalar);
		}

		inline Vector2 operator*(float scalar) {
			return Vector2(x * scalar, y * scalar);
		}

		inline bool operator==(const Vector2 &other) {
			return x == other.x && y == other.y;
		}

		inline bool operator!=(const Vector2 &other) {
			return x != other.x || y != other.y;
		}

	public:
		union {
			struct {
				float x, y;
			};
			float data[2];
		};
	};

	class Vector3 {
	public:
		Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
		Vector3(float xyz) : x(xyz), y(xyz) { }
		Vector3() : x(0), y(0), z(0) { }
		~Vector3() { }
	
	public:
		union {
			struct {
				float x, y, z;
			};
			float data[3];
		};
	};

}