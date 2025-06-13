#include "SceneRenderer.hpp"
#include "SceneView.hpp"
#include "Components/Tag.hpp"

#include <Arcane/Graphics/PBR/Renderer.hpp>

#include <iostream>

namespace Arcane {

	void SceneRenderer::Init() {
		Renderer::Init(GraphicsContext::GetCurrent());
	}

	void SceneRenderer::Draw() {
		EntityID cameraEntityID = GetCurrentScene()->GetMainEntity();
		if (!GetCurrentScene()->IsEntity(cameraEntityID)) {
			cameraEntityID = GetCurrentScene()->FindEntity<RenderCamera>();
			GetCurrentScene()->SetMainEntity(cameraEntityID);
		}

		if (!GetCurrentScene()->IsEntity(cameraEntityID)) return;

		Renderer::Begin(GetCurrentScene()->GetComponent<RenderCamera>(cameraEntityID));

		for (Entity e : SceneView<Transform, PointLight>()) {
			Renderer::AddLight(
				e.Get<Transform>().Position,
				e.Get<PointLight>()
			);
		}

		for (Entity e : SceneView<Transform, DirectionalLight>()) {
			const Transform &t = e.Get<Transform>();
			Vector3 direction = {
				Cos(ToRadians(t.Rotation.Y)) * Cos(ToRadians(t.Rotation.X)),
				Sin(ToRadians(t.Rotation.X)),
				Sin(ToRadians(t.Rotation.Y)) * Cos(ToRadians(t.Rotation.X))
			};

			Renderer::AddLight(
				direction,
				e.Get<DirectionalLight>()
			);
		}

		for (Entity e : SceneView<Transform, Mesh, Material>()) {
			Renderer::Submit(e.Get<Transform>(), e.Get<Mesh>(), e.Get<Material>());
		}

		Renderer::End();
	}

	void SceneRenderer::Shutdown() {
		Renderer::Shutdown();
	}

}