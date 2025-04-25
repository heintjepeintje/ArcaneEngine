#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/FrameStatistics.hpp>

#include "NativeGraphicsContext.hpp"
#include "NativePipeline.hpp"
#include "NativeMesh.hpp"
#include "NativeRenderPass.hpp"
#include "NativeFramebuffer.hpp"

namespace Arcane {

	class NativeRendererAPI {
	public:
		static Ref<NativeRendererAPI> Create(const Ref<NativeGraphicsContext> &context);

	public:
		NativeRendererAPI() { }
		virtual ~NativeRendererAPI() { }

		virtual void Begin() = 0;
		virtual void End() = 0;

		virtual void BeginRenderPass(const Ref<NativeRenderPass> &renderPass, const Ref<NativeFramebuffer> &framebuffer) = 0;
		virtual void EndRenderPass() = 0;

		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void SetClearDepth(float depth) = 0;
		virtual void SetClearStencil(uint16_t stencil) = 0;
		virtual void Clear() = 0;

		virtual void SetViewport(Rect2D viewport) = 0;
		virtual Rect2D GetViewport() const = 0;
		virtual void SetScissor(Rect2D scissor) = 0;
		virtual Rect2D GetScissor() const = 0;

		virtual void SetMesh(const Ref<NativeMesh> &mesh) = 0;
		virtual void SetPipeline(const Ref<NativePipeline> &pipeline) = 0;
		virtual void DrawIndexed(uint32_t instances, uint32_t count) = 0;

		virtual FrameStatistics GetFrameStatistics() const = 0;
	};

}