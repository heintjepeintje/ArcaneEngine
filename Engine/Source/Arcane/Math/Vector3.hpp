#pragma once

#include "Math.hpp"
#include "Vector2.hpp"

namespace Arcane {

	class Vector3 {
	public:
		static Vector3 Zero() { return Vector3(0, 0, 0); }
		static Vector3 MaxValue() { return Vector3(__FLT_MAX__, __FLT_MAX__, __FLT_MAX__); }
		static Vector3 MinValue() { return Vector3(-__FLT_MAX__, -__FLT_MAX__, -__FLT_MAX__); }

		static Vector3 Min(const Vector3 &a, const Vector3 &b) {
			return Vector3(
				Arcane::Min(a.X, b.X),
				Arcane::Min(a.Y, b.Y),
				Arcane::Min(a.Z, b.Z)
			);
		}

		static Vector3 Max(const Vector3 &a, const Vector3 &b) {
			return Vector3(
				Arcane::Max(a.X, b.X),
				Arcane::Max(a.Y, b.Y),
				Arcane::Max(a.Z, b.Z)
			);
		}

		static Vector3 Abs(const Vector3 &v) {
			return Vector3(
				Arcane::Abs(v.X),
				Arcane::Abs(v.Y),
				Arcane::Abs(v.Z)
			);
		}

		static Vector3 Clamp(const Vector3 &v, const Vector3 &min, const Vector3 &max) {
			return Vector3(
				Arcane::Clamp(v.X, min.X, max.X),
				Arcane::Clamp(v.Y, min.Y, max.Y),
				Arcane::Clamp(v.Z, min.Z, max.Z)
			);
		}

		static Vector3 Lerp(float t, const Vector3 &min, const Vector3 &max) {
			return Vector3(
				Arcane::Lerp(t, min.X, max.X),
				Arcane::Lerp(t, min.Y, max.Y),
				Arcane::Lerp(t, min.Z, max.Z)
			);
		}

		static float Length(const Vector3 &v) {
			return Sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
		}

		static float Dot(const Vector3 &a, const Vector3 &b) {
			return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
		}

		static Vector3 Cross(const Vector3 &a, const Vector3 &b) {
			return Vector3(
				a.Y * b.Z - a.Z * b.Y,
				a.Z * b.X - a.X * b.Z,
				a.X * b.Y - a.Y * b.X
			);
		}

		static Vector3 Normalize(const Vector3 &v) {
			const float length = Vector3::Length(v);
			return Vector3(
				v.X / length,
				v.Y / length,
				v.Z / length
			);
		}

	public:
		Vector3(float x, float y, float z) : X(x), Y(y), Z(z) { }
		Vector3(const Vector2 &xy, float z) : X(xy.X), Y(xy.Y), Z(z) { }
		Vector3(float x, const Vector2 &yz) : X(x), Y(yz.X), Z(yz.Y) { }
		Vector3(float xyz) : X(xyz), Y(xyz), Z(xyz) { }
		Vector3() : X(0), Y(0), Z(0) { }
		Vector3(const Vector3& other) : X(other.X), Y(other.Y), Z(other.Z) { }
		Vector3 &operator=(const Vector3 &other) { X = other.X; Y = other.Y; Z = other.Z; return *this; }
		~Vector3() { }

		inline Vector3 operator+(const Vector3 &other) const { return Vector3(X + other.X, Y + other.Y, Z + other.Z); }
		inline Vector3 operator-(const Vector3 &other) const { return Vector3(X - other.X, Y - other.Y, Z - other.Z); }
		inline Vector3 operator*(float scalar) const { return Vector3(X * scalar, Y * scalar, Z * scalar); }
		inline Vector3 operator/(float scalar) const { return Vector3(X / scalar, Y / scalar, Z / scalar); }

		inline Vector3 &operator+=(const Vector3 &other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }
		inline Vector3 &operator-=(const Vector3 &other) { X -= other.X; Y -= other.Y; Z -= other.Z; return *this; }
		inline Vector3 &operator*=(float scalar) { X *= scalar; Y *= scalar; Z *= scalar; return *this; }
		inline Vector3 &operator/=(float scalar) { X /= scalar; Y /= scalar; Z /= scalar; return *this; }

		inline bool operator==(const Vector3 &other) { return X == other.X && Y == other.Y && Z == other.Z; }
		inline bool operator!=(const Vector3 &other) { return X != other.X || Y != other.Y || Z != other.Z; }

	public:
		float X, Y, Z;
	};

	inline Vector3 operator*(float scalar, const Vector3 &v) {
		return Vector3(
			scalar * v.X,
			scalar * v.Y,
			scalar * v.Z
		);
	}

}