#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	class Transform {
	public:
		Transform() : Position(0.0f), Rotation(0.0f), Scale(1.0f) { }
		Transform(const Vector3 &position) : Position(position), Rotation(0), Scale(1) { }
		Transform(const Vector3 &position, const Vector3 &rotation) : Position(position), Rotation(rotation), Scale(1) { }
		~Transform() { }

		inline Vector3 GetDirection() {
			Vector3 direction = {
				Cos(ToRadians(Rotation.Y)) * Cos(ToRadians(Rotation.X)),
				Sin(ToRadians(Rotation.X)),
				Sin(ToRadians(Rotation.Y)) * Cos(ToRadians(Rotation.X))
			};
			
			return Vector3::Normalize(direction);
		}

		inline Matrix4 GetModelMatrix() const {
			Matrix4 modelMatrix = Matrix4::Identity();
			modelMatrix = Matrix4::Translate(modelMatrix, Position);

			// We invert the negate here because we transpose the matrix later
			modelMatrix = Matrix4::RotateX(modelMatrix, -ToRadians(Rotation.X));
			modelMatrix = Matrix4::RotateY(modelMatrix, -ToRadians(Rotation.Y));
			modelMatrix = Matrix4::RotateZ(modelMatrix, -ToRadians(Rotation.Z));
			modelMatrix = Matrix4::Scale(modelMatrix, Scale);
			return modelMatrix;
		}

	public:
		Vector3 Position;
		Vector3 Rotation;
		Vector3 Scale;
	};

}