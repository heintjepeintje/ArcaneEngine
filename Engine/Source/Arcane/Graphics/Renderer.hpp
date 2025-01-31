#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeRenderer.hpp>
#include "GraphicsContext.hpp"
#include "Pipeline.hpp"
#include "Mesh.hpp"
#include "RenderPass.hpp"
#include "Framebuffer.hpp"
#include "Color.hpp"

namespace Arcane {

	class RendererAPI {
	public:
		static RendererAPI Create(const GraphicsContext &context);

	public:
		RendererAPI() { }
		~RendererAPI() { }

		inline void SetClearColor(float r, float g, float b, float a) { GetNativeRendererAPI()->SetClearColor(r, g, b, a); }
		inline void SetClearColor(const Color &color) { GetNativeRendererAPI()->SetClearColor(color.R, color.G, color.B, color.A); }
		inline void Clear() { GetNativeRendererAPI()->Clear(); }

		inline void Begin()	{ GetNativeRendererAPI()->Begin(); }
		inline void End() { GetNativeRendererAPI()->End(); }

		inline void BeginRenderPass(const RenderPass &renderPass, const Framebuffer &framebuffer) { GetNativeRendererAPI()->BeginRenderPass(renderPass.GetNativeRenderPass(), framebuffer.GetNativeFramebuffer()); }
		inline void EndRenderPass() { GetNativeRendererAPI()->EndRenderPass(); }

		inline void SetViewport(Rect2D viewport) { GetNativeRendererAPI()->SetViewport(viewport); }
		inline Rect2D GetViewport() { return GetNativeRendererAPI()->GetViewport(); }
		inline void SetScissor(Rect2D scissor) { GetNativeRendererAPI()->SetScissor(scissor); }
		inline Rect2D GetScissor() { return GetNativeRendererAPI()->GetScissor(); }

		inline void SetMesh(const Mesh &mesh) { GetNativeRendererAPI()->SetMesh(mesh.GetNativeMesh()); }
		inline void DrawIndexed(uint32_t instances, uint32_t count) { GetNativeRendererAPI()->DrawIndexed(instances, count); }

		inline Ref<NativeRendererAPI> GetNativeRendererAPI() {
			AR_ASSERT(mNativeRendererAPI, "Native renderer API is invalid");
			return mNativeRendererAPI;
		}

	private:
		RendererAPI(const Ref<NativeRendererAPI> &rendererAPI) : mNativeRendererAPI(rendererAPI) { }

	private:
		Ref<NativeRendererAPI> mNativeRendererAPI;
	};

}