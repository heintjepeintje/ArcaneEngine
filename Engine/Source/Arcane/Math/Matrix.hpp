#pragma once

#include "Vector.hpp"

namespace Arcane {

	class Matrix4 {
	public:
		static Matrix4 Identity(float scalar = 1.0f) {
			return Matrix4(
				scalar, 0.0f, 0.0f, 0.0f,
				0.0f, scalar, 0.0f, 0.0f,
				0.0f, 0.0f, scalar, 0.0f,
				0.0f, 0.0f, 0.0f, scalar
			);
		}

		static Matrix4 Scale(const Vector3 &scale) {
			return Matrix4(
				scale.x, 0.0f, 0.0f, 0.0f,
				0.0f, scale.y, 0.0f, 0.0f,
				0.0f, 0.0f, scale.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		static Matrix4 Transpose(const Matrix4 &matrix) {
			return Matrix4(
				matrix.m00, matrix.m10, matrix.m20, matrix.m30,
				matrix.m01, matrix.m11, matrix.m21, matrix.m31,
				matrix.m02, matrix.m12, matrix.m22, matrix.m32,
				matrix.m03, matrix.m13, matrix.m23, matrix.m33
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