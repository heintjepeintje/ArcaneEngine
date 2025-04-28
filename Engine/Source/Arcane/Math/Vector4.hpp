#pragma once

#include <Arcane/Core.hpp>
#include "Math.hpp"
#include "Vector3.hpp"

namespace Arcane {

	class Vector4 {
	public:
		static Vector4 Zero() { return Vector4(0, 0, 0, 0); }
		static Vector4 MaxValue() { return Vector4(__FLT_MAX__); }
		static Vector4 MinValue() { return Vector4(-__FLT_MAX__); }

		static Vector4 Min(const Vector4 &a, const Vector4 &b) {
			return Vector4(
				Arcane::Min(a.X, b.X),
				Arcane::Min(a.Y, b.Y),
				Arcane::Min(a.Z, b.Z),
				Arcane::Min(a.W, b.W)
			);
		}

		static Vector4 Max(const Vector4 &a, const Vector4 &b) {
			return Vector4(
				Arcane::Max(a.X, b.X),
				Arcane::Max(a.Y, b.Y),
				Arcane::Max(a.Z, b.Z),
				Arcane::Max(a.W, b.W)
			);
		}

		static Vector4 Abs(const Vector4 &v) {
			return Vector4(
				Arcane::Abs(v.X),
				Arcane::Abs(v.Y),
				Arcane::Abs(v.Z),
				Arcane::Abs(v.W)
			);
		}

		static Vector4 Clamp(const Vector4 &v, const Vector4 &min, const Vector4 &max) {
			return Vector4(
				Arcane::Clamp(v.X, min.X, max.X),
				Arcane::Clamp(v.Y, min.Y, max.Y),
				Arcane::Clamp(v.Z, min.Z, max.Z),
				Arcane::Clamp(v.W, min.W, max.W)
			);
		}

		static Vector4 Lerp(float t, const Vector4 &min, const Vector4 &max) {
			return Vector4(
				Arcane::Lerp(t, min.X, max.X),
				Arcane::Lerp(t, min.Y, max.Y),
				Arcane::Lerp(t, min.Z, max.Z),
				Arcane::Lerp(t, min.W, max.W)
			);
		}

		static float Length(const Vector4 &v) {
			return Sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z + v.W * v.W);
		}

		static float Dot(const Vector4 &a, const Vector4 &b) {
			return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
		}

		static Vector4 Normalize(const Vector4 &v) {
			const float length = Vector4::Length(v);
			return Vector4(
				v.X / length,
				v.Y / length,
				v.Z / length,
				v.W / length
			);
		}

	public:
		Vector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
		Vector4(float xyzw) : X(xyzw), Y(xyzw), Z(xyzw), W(xyzw) { }
		Vector4(float x, const Vector3 &yzw) : X(x), Y(yzw.X), Z(yzw.Y), W(yzw.Z) { }
		Vector4(const Vector3 &xyz, float w) : X(xyz.X), Y(xyz.Y), Z(xyz.Z), W(w) { }
		Vector4() : X(0), Y(0), Z(0), W(0) { }
		Vector4(const Vector4& other) : X(other.X), Y(other.Y), Z(other.Z), W(other.W) { }
		Vector4 &operator=(const Vector4 &other) { X = other.X; Y = other.Y; Z = other.Z; W = other.W; return *this; }
		~Vector4() { }

		inline Vector4 operator+(const Vector4 &other) const { return Vector4(X + other.X, Y + other.Y, Z + other.Z, W + other.W); }
		inline Vector4 operator-(const Vector4 &other) const { return Vector4(X - other.X, Y - other.Y, Z - other.Z, W - other.W); }
		inline Vector4 operator*(float scalar) const { return Vector4(X * scalar, Y * scalar, Z * scalar, W * scalar); }
		inline Vector4 operator/(float scalar) const { return Vector4(X / scalar, Y / scalar, Z / scalar, W / scalar); }

		inline Vector4 &operator+=(const Vector4 &other) { X += other.X; Y += other.Y; Z += other.Z; W += other.W; return *this; }
		inline Vector4 &operator-=(const Vector4 &other) { X -= other.X; Y -= other.Y; Z -= other.Z; W -= other.W; return *this; }
		inline Vector4 &operator*=(float scalar) { X *= scalar; Y *= scalar; Z *= scalar; W *= scalar; return *this; }
		inline Vector4 &operator/=(float scalar) { X /= scalar; Y /= scalar; Z /= scalar; W /= scalar; return *this; }

		inline bool operator==(const Vector4 &other) { return X == other.X && Y == other.Y; }
		inline bool operator!=(const Vector4 &other) { return X != other.X || Y != other.Y; }

	public:
		float X, Y, Z, W;
	};

	inline Vector4 operator*(float scalar, const Vector4 &v) {
		return Vector4(
			scalar * v.X,
			scalar * v.Y,
			scalar * v.Z,
			scalar * v.W
		);
	}

}