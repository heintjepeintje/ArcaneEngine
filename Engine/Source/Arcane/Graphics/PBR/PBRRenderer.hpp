#pragma once

#include <Arcane/Graphics/GraphicsContext.hpp>
#include <Arcane/Graphics/Camera.hpp>
#include <Arcane/Graphics/Mesh.hpp>
#include <Arcane/Graphics/Transform.hpp>
#include "PBRMaterial.hpp"
#include <Arcane/Graphics/Light.hpp>

namespace Arcane {

	class PBRRenderer {
	public:
		static void Init(const GraphicsContext &context);
		static void Shutdown();

		static void Begin(const Camera3D &camera);
		static void AddLight(const Transform &transform, const PointLight &light);
		static void AddLight(const Transform &transform, const DirectionalLight &light);
		static void Submit(const Transform &transform, const Mesh &mesh, const PBRMaterial &material);
		static void End();
	};

}