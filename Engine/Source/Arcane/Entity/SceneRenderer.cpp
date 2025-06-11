#include "SceneRenderer.hpp"
#include "SceneView.hpp"
#include "Components/Tag.hpp"

#include <Arcane/Graphics/PBR/PBRRenderer.hpp>

#include <iostream>

namespace Arcane {

	void SceneRenderer::Init() {
		PBRRenderer::Init(GraphicsContext::GetCurrent());
	}

	void SceneRenderer::Draw() {
		EntityID cameraEntityID = GetCurrentScene()->GetMainEntity();
		if (!GetCurrentScene()->IsEntity(cameraEntityID)) {
			cameraEntityID = GetCurrentScene()->FindEntity<RenderCamera>();
			GetCurrentScene()->SetMainEntity(cameraEntityID);
		}

		if (!GetCurrentScene()->IsEntity(cameraEntityID)) return;

		PBRRenderer::Begin(GetCurrentScene()->GetComponent<RenderCamera>(cameraEntityID));

		for (Entity e : SceneView<Transform, PointLight>()) {
			PBRRenderer::AddLight(
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

			PBRRenderer::AddLight(
				direction,
				e.Get<DirectionalLight>()
			);
		}

		for (Entity e : SceneView<Transform, Mesh, PBRMaterial>()) {
			PBRRenderer::Submit(e.Get<Transform>(), e.Get<Mesh>(), e.Get<PBRMaterial>());
		}

		PBRRenderer::End();
	}

	void SceneRenderer::Shutdown() {
		PBRRenderer::Shutdown();
	}

}