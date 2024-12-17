#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeRenderer.hpp>
#include "GraphicsContext.hpp"
#include "Pipeline.hpp"
#include "Mesh.hpp"

namespace Arcane {

	class RendererAPI {
	public:
		static RendererAPI Create(const GraphicsContext &context);

	public:
		inline void SetClearColor(float r, float g, float b, float a) { mNativeRenderer->SetClearColor(r, g, b, a); }
		inline void Clear() { mNativeRenderer->Clear(); }

		inline void SetViewport(Rect2D viewport) { mNativeRenderer->SetViewport(viewport); }
		inline void SetScissor(Rect2D scissor) { mNativeRenderer->SetScissor(scissor); }

		inline void SetPipeline(const Pipeline &pipeline) { mNativeRenderer->SetPipeline(pipeline.GetNativePipeline()); }
		inline void SetMesh(const Mesh &mesh) { mNativeRenderer->SetMesh(mesh.GetNativeMesh()); }
		inline void DrawIndexed(uint32_t count) { mNativeRenderer->DrawIndexed(count); }

	private:
		RendererAPI(const std::shared_ptr<NativeRendererAPI> &renderer) : mNativeRenderer(renderer) { }

	private:
		std::shared_ptr<NativeRendererAPI> mNativeRenderer;
	};

}