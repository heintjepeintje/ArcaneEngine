#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeRenderer.hpp>
#include "OpenGLCore.hpp"
#include "OpenGLGraphicsContext.hpp"
#include "OpenGLPipeline.hpp"
#include "OpenGLMesh.hpp"

namespace Arcane {

	class OpenGLRendererAPI : public NativeRendererAPI {
	public:
		OpenGLRendererAPI(const std::shared_ptr<OpenGLGraphicsContext> &context);
		~OpenGLRendererAPI();

		virtual void SetClearColor(float r, float g, float b, float a) override;
		virtual void Clear() override;

		virtual void SetViewport(Rect2D viewport) override;;
		virtual void SetScissor(Rect2D scissor) override;

		virtual void SetPipeline(const std::shared_ptr<NativePipeline> &pipeline) override;
		virtual void SetMesh(const std::shared_ptr<NativeMesh> &mesh) override;
		virtual void DrawIndexed(uint32_t count) override;

	private:
		std::shared_ptr<OpenGLGraphicsContext> mContext;
		std::shared_ptr<OpenGLPipeline> mPipeline;
		std::shared_ptr<OpenGLMesh> mMesh;

		Rect2D mViewport;
		Rect2D mScissor;
	};

}