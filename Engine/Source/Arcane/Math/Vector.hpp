#pragma once

#include <Arcane/Core.hpp>
#include "Math.hpp"

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

		inline Vector2 operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }
		inline Vector2 operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }
		inline Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
		inline Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }
		inline void operator+=(const Vector2 &other) { x += other.x; y += other.y; }
		inline void operator-=(const Vector2 &other) { x -= other.x; y -= other.y; }
		inline void operator*=(float scalar) { x *= scalar; y *= scalar; }
		inline void operator/=(float scalar) { x /= scalar; y /= scalar; }
		inline bool operator==(const Vector2 &other) const { return x == other.x && y == other.y; }
		inline bool operator!=(const Vector2 &other) const { return x != other.x || y != other.y; }

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
		inline static Vector3 Cross(const Vector3 &a, const Vector3 &b) {
			return Vector3(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			);
		}

		inline static float Length(const Vector3 &v) { return Sqrt(v.x * v.x + v.y * v.y + v.z * v.z); } 
		inline static Vector3 Normalize(const Vector3 &v) { return v / Length(v); }

	public:
		Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
		Vector3(float xyz) : x(xyz), y(xyz) { }
		Vector3() : x(0), y(0), z(0) { }
		~Vector3() { }

		inline Vector3 operator+(const Vector3 &other) const { return Vector3(x + other.x, y + other.y, z + other.z); }
		inline Vector3 operator-(const Vector3 &other) const { return Vector3(x - other.x, y - other.y, z - other.z); }
		inline Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
		inline Vector3 operator/(float scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }

		inline void operator+=(const Vector3 &other) { x += other.x; y += other.y; z += other.z; }
		inline void operator-=(const Vector3 &other) { x -= other.x; y -= other.y; z -= other.z; }
		inline void operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; }
		inline void operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; }

		inline bool operator==(const Vector3 &other) const { return x == other.x && y == other.y && z == other.z; }
		inline bool operator!=(const Vector3 &other) const { return x != other.x || y != other.y || z != other.z; }
	
	public:
		union {
			struct {
				float x, y, z;
			};
			float data[3];
		};
	};

	class Vector4 {
	public:
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
		Vector4(float xyzw) : x(xyzw), y(xyzw), z(xyzw), w(xyzw) { }
		Vector4() : x(0), y(0), z(0), w(0) { }
		~Vector4() { }
		
	public:
		union {
			struct {
				float x, y, z, w;
			};
			float data[4];
		};
	};

}