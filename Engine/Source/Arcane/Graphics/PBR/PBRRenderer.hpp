#pragma once

#include <Arcane/Graphics/GraphicsContext.hpp>
#include <Arcane/Graphics/Camera.hpp>
#include <Arcane/Graphics/Mesh.hpp>
#include <Arcane/Graphics/Transform.hpp>
#include <Arcane/Graphics/Renderer.hpp>
#include <Arcane/Graphics/Light.hpp>

#include "PBRMaterial.hpp"
#include "RenderCamera.hpp"

namespace Arcane {

	class PBRRenderer {
	public:
		static void Init(const GraphicsContext &context);
		static void Reload();
		static void Shutdown();

		static void Begin(const RenderCamera &camera);
		static void AddLight(const Vector3 &position, const PointLight &light);
		static void AddLight(const Vector3 &direction, const DirectionalLight &light);
		static void Submit(const Transform &transform, const Mesh &mesh, const PBRMaterial &material);
		static void End();

		static RendererAPI GetRenderer();
	};

}