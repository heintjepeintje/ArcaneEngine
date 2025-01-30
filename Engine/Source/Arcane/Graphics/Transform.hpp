#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	class Transform {
	public:
		Transform() : Position(0.0f), Rotation(0.0f), Scale(1.0f) { }
		~Transform() { }

		inline Matrix4 GetModelMatrix() const {
			Matrix4 modelMatrix = Matrix4::Identity();
			modelMatrix = Matrix4::Translate(modelMatrix, Position);

			// We invert the rotation here because we transpose the matrix later
			modelMatrix = Matrix4::RotateX(modelMatrix, -Rotation.X);
			modelMatrix = Matrix4::RotateY(modelMatrix, -Rotation.Y);
			modelMatrix = Matrix4::RotateZ(modelMatrix, -Rotation.Z);
			modelMatrix = Matrix4::Scale(modelMatrix, Scale);
			return modelMatrix;
		}

	public:
		Vector3 Position;
		Vector3 Rotation;
		Vector3 Scale;
	};

}