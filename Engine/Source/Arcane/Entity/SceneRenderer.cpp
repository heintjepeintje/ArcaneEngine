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
		Renderer::Begin(
			SceneView<RenderCamera>()
				.FindFirst([](Entity e, RenderCamera &camera) { return camera.IsActive(); })
				.Get<RenderCamera>().GetCamera()
		);

		SceneView<Transform, PointLight>().ForEach([](Transform &transform, PointLight &pointLight) {
			Renderer::AddLight(transform.Position, pointLight);
		});

		SceneView<Transform, DirectionalLight>().ForEach([](Transform &transform, DirectionalLight &dirLight) {
			Renderer::AddLight(transform.GetDirection(), dirLight);
		});

		SceneView<Transform, Mesh, Material>().ForEach(Renderer::Submit);

		Renderer::End();
	}

	void SceneRenderer::Shutdown() {
		Renderer::Shutdown();
	}

}