#pragma once

#include <Arcane/Core.hpp>
#include "Vector3.hpp"

namespace Arcane {

	class Matrix3 {
	public:
		static Matrix3 Identity(float identity = 1.0f) {
			return Matrix3(
				{ identity, 0.0f, 0.0f },
				{ 0.0f, identity, 0.0f },
				{ 0.0f, 0.0f, identity }
			);
		}

	public:
		Matrix3() { }
		Matrix3(const Vector3 &r0, const Vector3 &r1, const Vector3 &r2) {
			Rows[0] = r0;
			Rows[1] = r1;
			Rows[2] = r2;
		}
		Matrix3(const Matrix3 &m) {
			std::memcpy(Data, m.Data, 9 * sizeof(float));
		}
		Matrix3 &operator=(const Matrix3 &other) {
			std::memcpy(Data, other.Data, 9 * sizeof(float));
			return *this;
		}
		~Matrix3() { }

		inline Matrix3 operator+(const Matrix3 &m) const {
			return Matrix3(
				Rows[0] + m.Rows[0],
				Rows[1] + m.Rows[1],
				Rows[2] + m.Rows[2]
			);
		}

		inline Matrix3 operator-(const Matrix3 &m) const {
			return Matrix3(
				Rows[0] - m.Rows[0],
				Rows[1] - m.Rows[1],
				Rows[2] - m.Rows[2]
			);
		}

		inline Matrix3 operator*(const Matrix3 &m) const {
			Matrix3 result;
			for (uint32_t i = 0; i < 3; i++) {
				for (uint32_t j = 0; j < 3; j++) {
					float sum = 0.0f;
					for (uint32_t k = 0; k < 3; k++) {
						sum += Data[i][k] * m.Data[k][j];
					}
					result.Data[i][j] = sum;
				}
			}
			return result;
		}

		inline Matrix3 &operator+=(const Matrix3 &m) {
			Rows[0] += m.Rows[0];
			Rows[1] += m.Rows[1];
			Rows[2] += m.Rows[2];
			return *this;
		}

		inline Matrix3 &operator-=(const Matrix3 &m) {
			Rows[0] -= m.Rows[0];
			Rows[1] -= m.Rows[1];
			Rows[2] -= m.Rows[2];
			return *this;
		}

		inline Matrix3 &operator*=(const Matrix3 &m) {
			Matrix3 result;
			for (uint32_t i = 0; i < 3; i++) {
				for (uint32_t j = 0; j < 3; j++) {
					float sum = 0.0f;
					for (uint32_t k = 0; k < 3; k++) {
						sum += Data[i][k] * m.Data[k][j];
					}
					result.Data[i][j] = sum;
				}
			}

			for (uint32_t i = 0; i < 3; i++) {
				Rows[i] = result.Rows[i];
			}
			return *this;
		}

	public:
		union {
			struct {
				float M00, M01, M02;
				float M10, M11, M12;
				float M20, M21, M22;
			};
			Vector3 Rows[3] = {};
			float Data[3][3];
		};
	};

}