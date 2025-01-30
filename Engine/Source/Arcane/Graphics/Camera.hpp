#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Math/Vector3.hpp>
#include <Arcane/Math/Matrix4.hpp>

namespace Arcane {

	class Camera {
	public:
		Camera() { }
		virtual ~Camera() { }

		virtual Matrix4 GetProjectionMatrix() const = 0;
		virtual Matrix4 GetViewMatrix() const = 0;
	};

	class Camera3D : public Camera {
	public:
		Camera3D() : FOVy(0.0f), AspectRatio(0.0f), Near(0.0f), Far(0.0f) { }
		Camera3D(float fovy, const Vector2 &size, float near, float far) : FOVy(fovy), AspectRatio(size.X / size.Y), Near(near), Far(far) { }
		Camera3D(float fovy, float aspect, float near, float far) : FOVy(fovy), AspectRatio(aspect), Near(near), Far(far) { }
		~Camera3D() { }

		inline virtual Matrix4 GetProjectionMatrix() const override {
			return Matrix4::PerspectiveLH_ZO(FOVy, AspectRatio, Near, Far);
		}

		inline virtual Matrix4 GetViewMatrix() const override {
			return Matrix4::LookAtLH(Position, Position + Front, Up);
		}

	public:
		Vector3 Position = Vector3(0);
		Vector3 Front = Vector3(0, 0, 1);
		Vector3 Up = Vector3(0, 1, 0);

		float FOVy;
		float AspectRatio;
		float Near, Far;
	};

}