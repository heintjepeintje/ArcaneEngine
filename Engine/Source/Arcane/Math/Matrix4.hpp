#pragma once

#include <Arcane/Core.hpp>
#include "Vector4.hpp"

namespace Arcane {

	class Matrix4 {
	public:
		static Matrix4 Identity(float scale = 1.0f) {
			return Matrix4(
				{ scale, 0, 0, 0 },
				{ 0, scale, 0, 0 },
				{ 0, 0, scale, 0 },
				{ 0, 0, 0, scale }
			);
		}

		static Matrix4 Transpose(const Matrix4 &matrix) {
			return Matrix4(
				{ matrix.M00, matrix.M10, matrix.M20, matrix.M30 },
				{ matrix.M01, matrix.M11, matrix.M21, matrix.M31 },
				{ matrix.M02, matrix.M12, matrix.M22, matrix.M32 },
				{ matrix.M03, matrix.M13, matrix.M23, matrix.M33 }
			);
		}

		static Matrix4 Translate(const Matrix4 &m, const Vector3 &translation) {
			Matrix4 translate = {
				{ 1, 0, 0, translation.X },
				{ 0, 1, 0, translation.Y },
				{ 0, 0, 1, translation.Z },
				{ 0, 0, 0, 1 }
			};

			return m * translate;
		}

		static Matrix4 RotateX(const Matrix4 &m, float angle) {
			const float s = Sin(angle);
			const float c = Cos(angle);
		
			const Matrix4 rotate = {
				{ 1, 0, 0, 0 },
				{ 0, c, -s, 0 },
				{ 0, s, c, 0 },
				{ 0, 0, 0, 1 }	
			};

			return m * rotate;
		}

		static Matrix4 RotateY(const Matrix4 &m, float angle) {
			const float s = Sin(angle);
			const float c = Cos(angle);

			const Matrix4 rotate = {
				{ c, 0, s, 0 },
				{ 0, 1, 0, 0 },
				{ -s, 0, c, 0 },
				{ 0, 0, 0, 1}
			};

			return m * rotate;
		}

		static Matrix4 RotateZ(const Matrix4 &m, float angle) {
			const float s = Sin(angle);
			const float c = Cos(angle);

			const Matrix4 rotate = {
				{ c, -s, 0, 0 },
				{ s, c, 0, 0 },
				{ 0, 0, 1, 0 },
				{ 0, 0, 0, 1 }
			};

			return m * rotate;
		}

		static Matrix4 Scale(const Matrix4 &m, const Vector3 &scale) {
			return m * Matrix4(
				{ scale.X, 0, 0, 0 },
				{ 0, scale.Y, 0, 0 },
				{ 0, 0, scale.Z, 0 },
				{ 0, 0, 0, 1 }
			);
		}

		static Matrix4 LookAtLH(const Vector3 &position, const Vector3 &target, const Vector3 &up) {
			const Vector3 n = Vector3::Normalize(target - position);
			const Vector3 u = Vector3::Normalize(Vector3::Cross(up, n));
			const Vector3 v = Vector3::Cross(n, u);

			return Matrix4(
				{ u.X, u.Y, u.Z, -Vector3::Dot(u, position) },
				{ v.X, v.Y, v.Z, -Vector3::Dot(v, position) },
				{ n.X, n.Y, n.Z, -Vector3::Dot(n, position) },
				{ 0, 0, 0, 1 }
			);
		}

		static Matrix4 LookAtRH(const Vector3 &position, const Vector3 &target, const Vector3 &up) {
			const Vector3 n = Vector3::Normalize(target - position);
			const Vector3 u = Vector3::Normalize(Vector3::Cross(n, up));
			const Vector3 v = Vector3::Cross(u, n);

			return Matrix4(
				{ u.X, u.Y, u.Z, -Vector3::Dot(u, position) },
				{ v.X, v.Y, v.Z, -Vector3::Dot(v, position) },
				{ -n.X, -n.Y, -n.Z,  Vector3::Dot(n, position) },
				{ 0, 0, 0, 1 }
			);
		}

		static Matrix4 PerspectiveLH_ZO(float fov, float aspect, float near, float far) {
			const float tanHalfFOV = Tan(fov * 0.5f);

			return Matrix4(
				{ 1.0f / (aspect * tanHalfFOV), 0.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f / tanHalfFOV, 0.0f, 0.0f, },
				{ 0.0f, 0.0f, far / (far - near), -(far * near) / (far - near) },
				{ 0.0f, 0.0f, 1.0f, 0.0f }
			);
		}

		static Matrix4 PerspectiveLH_NO(float fov, float aspect, float near, float far) {
			const float tanHalfFOV = Tan(fov * 0.5f);

			return Matrix4(
				{ 1.0f / (aspect * tanHalfFOV), 0.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f / tanHalfFOV, 0.0f, 0.0f, },
				{ 0.0f, 0.0f, (far + near) / (far - near), -(2.0f * far * near) / (far - near) },
				{ 0.0f, 0.0f, 1.0f, 0.0f }
			);
		}

		static Matrix4 PerspectiveRH_NO(float fov, float aspect, float near, float far) {
			const float tanHalfFOV = Tan(fov * 0.5f);

			return Matrix4(
				{ 1.0f / (aspect * tanHalfFOV), 0.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f / tanHalfFOV, 0.0f, 0.0f, },
				{ 0.0f, 0.0f, -(far + near) / (far - near), -(2.0f * far * near) / (far - near) },
				{ 0.0f, 0.0f, -1.0f, 0.0f }
			);
		}

		static Matrix4 OrthoLH_ZO(float left, float right, float top, float bottom, float near, float far) {
			return Matrix4(
				{ 2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left) },
				{ 0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom) },
				{ 0.0f, 0.0f, 1.0f / (far - near), -near / (far - near) },
				{ 0.0f, 0.0f, 0.0f, 1.0f }
			);
		}

		static Matrix4 OrthoLH_NO(float left, float right, float top, float bottom, float near, float far) {
			return Matrix4(
				{ 2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left) },
				{ 0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom) },
				{ 0.0f, 0.0f, 2.0f / (far - near), -(far + near) / (far - near) },
				{ 0.0f, 0.0f, 0.0f, 1.0f }
			);
		}

		static Matrix4 OrthoRH_ZO(float left, float right, float top, float bottom, float near, float far) {
			return Matrix4(
				{ 2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left) },
				{ 0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom) },
				{ 0.0f, 0.0f, 1.0f / (far - near), -near / (far - near) },
				{ 0.0f, 0.0f, 0.0f, 1.0f }
			);
		}

		static Matrix4 OrthoRH_NO(float left, float right, float top, float bottom, float near, float far) {
			return Matrix4(
				{ 2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left) },
				{ 0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom) },
				{ 0.0f, 0.0f, 2.0f / (far - near), -(far + near) / (far - near) },
				{ 0.0f, 0.0f, 0.0f, 1.0f }
			);
		}

	public:
		Matrix4() { }
		Matrix4(const Vector4 &r0, const Vector4 &r1, const Vector4 &r2, const Vector4 &r3) {
			Rows[0] = r0;
			Rows[1] = r1;
			Rows[2] = r2;
			Rows[3] = r3;
		}
		Matrix4(const Matrix4 &m) {
			std::memcpy(Data, m.Data, 16 * sizeof(float));
		}
		Matrix4 &operator=(const Matrix4 &other) {
			std::memcpy(Data, other.Data, 16 * sizeof(float));
			return *this;
		}
		~Matrix4() { }

		inline Matrix4 operator+(const Matrix4 &m) const {
			return Matrix4(
				Rows[0] + m.Rows[0],
				Rows[1] + m.Rows[1],
				Rows[2] + m.Rows[2],
				Rows[3] + m.Rows[3]
			);
		}

		inline Matrix4 operator-(const Matrix4 &m) const {
			return Matrix4(
				Rows[0] - m.Rows[0],
				Rows[1] - m.Rows[1],
				Rows[2] - m.Rows[2],
				Rows[3] - m.Rows[3]
			);
		}

		inline Matrix4 operator*(const Matrix4 &m) const {
			Matrix4 result;
			for (uint32_t i = 0; i < 4; i++) {
				for (uint32_t j = 0; j < 4; j++) {
					float sum = 0.0f;
					for (uint32_t k = 0; k < 4; k++) {
						sum += Data[i][k] * m.Data[k][j];
					}
					result.Data[i][j] = sum;
				}
			}
			return result;
		}

		inline Vector4 operator*(const Vector4 &m) const {
			Vector4 result;
			for (uint32_t i = 0; i < 4; i++) {
				float sum = 0.0f;
				for (uint32_t k = 0; k < 4; k++) {
					sum += Data[i][k] * m.Data[k];
				}
				result.Data[i] = sum;
			}
			return result;
		}

		inline Matrix4 &operator+=(const Matrix4 &m) {
			Rows[0] += m.Rows[0];
			Rows[1] += m.Rows[1];
			Rows[2] += m.Rows[2];
			Rows[3] += m.Rows[3];
			return *this;
		}

		inline Matrix4 &operator-=(const Matrix4 &m) {
			Rows[0] -= m.Rows[0];
			Rows[1] -= m.Rows[1];
			Rows[2] -= m.Rows[2];
			Rows[3] -= m.Rows[3];
			return *this;
		}

		inline Matrix4 &operator*=(const Matrix4 &m) {
			Matrix4 result;
			for (uint32_t i = 0; i < 4; i++) {
				for (uint32_t j = 0; j < 4; j++) {
					float sum = 0.0f;
					for (uint32_t k = 0; k < 4; k++) {
						sum += Data[i][k] * m.Data[k][j];
					}
					result.Data[i][j] = sum;
				}
			}

			for (uint32_t i = 0; i < 4; i++) {
				Rows[i] = result.Rows[i];
			}
			return *this;
		}

	public:
		union {
			struct {
				float M00, M01, M02, M03;
				float M10, M11, M12, M13;
				float M20, M21, M22, M23;
				float M30, M31, M32, M33;
			};
			Vector4 Rows[4] = {};
			float Data[4][4];
		};
	};

}