#pragma once

#include "Math.hpp"

namespace Arcane {

	class Vector2 {
	public:
		static Vector2 Zero() { return Vector2(0, 0); }
		static Vector2 MaxValue() { return Vector2(__FLT_MAX__, __FLT_MAX__); }
		static Vector2 MinValue() { return Vector2(-__FLT_MAX__, -__FLT_MAX__); }

		static Vector2 Min(const Vector2 &a, const Vector2 &b) {
			return Vector2(
				Arcane::Min(a.X, b.X),
				Arcane::Min(a.Y, b.Y)
			);
		}

		static Vector2 Max(const Vector2 &a, const Vector2 &b) {
			return Vector2(
				Arcane::Max(a.X, b.X),
				Arcane::Max(a.Y, b.Y)
			);
		}

		static Vector2 Abs(const Vector2 &v) {
			return Vector2(
				Arcane::Abs(v.X),
				Arcane::Abs(v.Y)
			);
		}

		static Vector2 Clamp(const Vector2 &v, const Vector2 &min, const Vector2 &max) {
			return Vector2(
				Arcane::Clamp(v.X, min.X, max.X),
				Arcane::Clamp(v.Y, min.Y, max.Y)
			);
		}

		static Vector2 Lerp(float t, const Vector2 &min, const Vector2 &max) {
			return Vector2(
				Arcane::Lerp(t, min.X, max.X),
				Arcane::Lerp(t, min.Y, max.Y)
			);
		}

		static float Length(const Vector2 &v) {
			return Sqrt(v.X * v.X + v.Y * v.Y);
		}

		static float Dot(const Vector2 &a, const Vector2 &b) {
			return a.X * b.X + a.Y * b.Y;
		}

		static Vector2 Normalize(const Vector2 &v) {
			const float length = Vector2::Length(v);
			return Vector2(
				v.X / length,
				v.Y / length
			);
		}

	public:
		Vector2(float x, float y) : X(x), Y(y) { }
		Vector2(float xy) : X(xy), Y(xy) { }
		Vector2() : X(0), Y(0) { }
		Vector2(const Vector2& other) : X(other.X), Y(other.Y) { }
		Vector2 &operator=(const Vector2 &other) { X = other.X; Y = other.Y; return *this; }
		~Vector2() { }

		inline Vector2 operator+(const Vector2 &other) const { return Vector2(X + other.X, Y + other.Y); }
		inline Vector2 operator-(const Vector2 &other) const { return Vector2(X - other.X, Y - other.Y); }
		inline Vector2 operator*(float scalar) const { return Vector2(X * scalar, Y * scalar); }
		inline Vector2 operator/(float scalar) const { return Vector2(X / scalar, Y / scalar); }

		inline Vector2 &operator+=(const Vector2 &other) { X += other.X; Y += other.Y; return *this; }
		inline Vector2 &operator-=(const Vector2 &other) { X -= other.X; Y -= other.Y; return *this; }
		inline Vector2 &operator*=(float scalar) { X *= scalar; Y *= scalar; return *this; }
		inline Vector2 &operator/=(float scalar) { X /= scalar; Y /= scalar; return *this; }

		inline bool operator==(const Vector2 &other) { return X == other.X && Y == other.Y; }
		inline bool operator!=(const Vector2 &other) { return X != other.X || Y != other.Y; }

	public:
		float X, Y;
	};

	inline Vector2 operator*(float scalar, const Vector2 &v) {
		return Vector2(
			scalar * v.X,
			scalar * v.Y
		);
	}

}