#pragma once

#include "Math.hpp"
#include "Vector.hpp"

namespace Arcane {

	class Matrix4 {
	public:
		inline static Matrix4 Identity(float scalar = 1.0f) {
			return Matrix4(
				scalar, 0.0f, 0.0f, 0.0f,
				0.0f, scalar, 0.0f, 0.0f,
				0.0f, 0.0f, scalar, 0.0f,
				0.0f, 0.0f, 0.0f, scalar
			);
		}

		inline static Matrix4 Identity(const Vector4 &scalar) {
			return Matrix4(
				scalar.x, 0.0f, 0.0f, 0.0f,
				0.0f, scalar.y, 0.0f, 0.0f,
				0.0f, 0.0f, scalar.z, 0.0f,
				0.0f, 0.0f, 0.0f, scalar.w
			);
		}

		inline static Matrix4 Scale(const Vector3 &scale) {
			return Matrix4(
				scale.x, 0.0f, 0.0f, 0.0f,
				0.0f, scale.y, 0.0f, 0.0f,
				0.0f, 0.0f, scale.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		inline static Matrix4 RotateX(float angle) {
			return Matrix4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, Cos(angle), -Sin(angle), 0.0f,
				0.0f, Sin(angle), Cos(angle), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		inline static Matrix4 RotateY(float angle) {
			return Matrix4(
				Cos(angle), 0.0f, Sin(angle), 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				-Sin(angle), 0.0f, Cos(angle), 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f	
			);
		}

		inline static Matrix4 RotateZ(float angle) {
			return Matrix4(
				Cos(angle), -Sin(angle), 0.0f, 0.0f,
				Sin(angle),  Cos(angle), 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f	
			);
		}

		inline static Matrix4 Translate(const Vector3 &translation) {
			return Matrix4(
				1.0f, 0.0f, 0.0f, translation.x,
				0.0f, 1.0f, 0.0f, translation.y,
				0.0f, 0.0f, 1.0f, translation.z,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		inline static Matrix4 Transpose(const Matrix4 &matrix) {
			return Matrix4(
				matrix.m00, matrix.m10, matrix.m20, matrix.m30,
				matrix.m01, matrix.m11, matrix.m21, matrix.m31,
				matrix.m02, matrix.m12, matrix.m22, matrix.m32,
				matrix.m03, matrix.m13, matrix.m23, matrix.m33
			);
		}

		inline static Matrix4 Perspective(float fovy, float aspect, float near, float far) {
			const float tanHalfFovY = Tan(fovy / 2.0f);
			return Matrix4(
				1.0f / (aspect * tanHalfFovY), 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f / tanHalfFovY, 0.0f, 0.0f,
				0.0f, 0.0f, (far + near) / (far - near), -(2.0f * far * near) / (far - near),
				0.0f, 0.0f, 1.0f, 0.0f
			);
		}

		inline static Matrix4 Perspective(float fovy, Vector2 size, float near, float far) {
			return Perspective(fovy, size.x / size.y, near, far);
		}

		inline static Matrix4 LookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up) {
			const Vector3 N = Vector3::Normalize(target - eye);
			const Vector3 V = Vector3::Normalize(up);
			const Vector3 U = Vector3::Cross(V, N);

			return Matrix4(
				U.x, U.y, U.z, -eye.x,
				V.x, V.y, V.z, -eye.y,
				N.x, N.y, N.z, -eye.z,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

	public:
		Matrix4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
		) : 
			m00(m00), m01(m01), m02(m02), m03(m03),
			m10(m10), m11(m11), m12(m12), m13(m13),
			m20(m20), m21(m21), m22(m22), m23(m23),
			m30(m30), m31(m31), m32(m32), m33(m33)
		{ }

	public:
		union {
			float elements[4 * 4];
			struct {
				float row0[4];
				float row1[4];
				float row2[4];
				float row3[4];
			};
			struct {
				float m00, m01, m02, m03;
				float m10, m11, m12, m13;
				float m20, m21, m22, m23;
				float m30, m31, m32, m33;
			};
		};
	};

}