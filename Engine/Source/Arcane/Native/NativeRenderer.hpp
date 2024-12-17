#pragma once

#include <Arcane/Core.hpp>

#include "NativeGraphicsContext.hpp"
#include "NativePipeline.hpp"
#include "NativeMesh.hpp"

namespace Arcane {

	class NativeRendererAPI {
	public:
		static std::shared_ptr<NativeRendererAPI> Create(const std::shared_ptr<NativeGraphicsContext> &context);

	public:
		NativeRendererAPI() { }
		virtual ~NativeRendererAPI() { }

		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void Clear() = 0;

		virtual void SetViewport(Rect2D viewport) = 0;
		virtual void SetScissor(Rect2D scissor) = 0;

		virtual void SetPipeline(const std::shared_ptr<NativePipeline> &pipeline) = 0;
		virtual void SetMesh(const std::shared_ptr<NativeMesh> &mesh) = 0;
		virtual void DrawIndexed(uint32_t count) = 0;
	};

}