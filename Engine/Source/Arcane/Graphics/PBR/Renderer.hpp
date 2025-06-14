#pragma once

#include <Arcane/Graphics/Base/GraphicsContext.hpp>
#include <Arcane/Graphics/Camera.hpp>
#include <Arcane/Graphics/Base/Mesh.hpp>
#include <Arcane/Graphics/Transform.hpp>
#include <Arcane/Graphics/Base/RendererAPI.hpp>
#include <Arcane/Graphics/Light.hpp>

#include "Material.hpp"
#include "RenderCamera.hpp"

namespace Arcane {

	class Renderer {
	public:
		static void Init(const GraphicsContext &context);
		static void Reload();
		static void Shutdown();

		static void Begin(const RenderCamera &camera);
		static void AddLight(const Vector3 &position, const PointLight &light);
		static void AddLight(const Vector3 &direction, const DirectionalLight &light);
		static void Submit(const Transform &transform, const Mesh &mesh, const Material &material);
		static void End();

		static RendererAPI GetRenderer();
	};

}