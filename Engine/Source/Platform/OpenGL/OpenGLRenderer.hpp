#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeRenderer.hpp>

#include "OpenGLCore.hpp"
#include "OpenGLGraphicsContext.hpp"
#include "OpenGLPipeline.hpp"
#include "OpenGLMesh.hpp"
#include "OpenGLFramebuffer.hpp"
#include "OpenGLRenderPass.hpp"

namespace Arcane {

	class OpenGLRendererAPI : public NativeRendererAPI {
	public:
		OpenGLRendererAPI(const Ref<OpenGLGraphicsContext> &context);
		~OpenGLRendererAPI();

		virtual void Begin() override;
		virtual void End() override;

		virtual void BeginRenderPass(const Ref<NativeRenderPass> &renderPass, const Ref<NativeFramebuffer> &framebuffer) override;
		virtual void EndRenderPass() override;

		virtual void SetClearColor(float r, float g, float b, float a) override;
		virtual void SetClearDepth(float depth) override;
		virtual void SetClearStencil(uint16_t stencil) override;
		virtual void Clear() override;

		virtual void SetViewport(Rect2D viewport) override;
		virtual Rect2D GetViewport() const { return mOutputViewport; }
		virtual void SetScissor(Rect2D scissor) override;
		virtual Rect2D GetScissor() const { return mOutputScissor; }

		virtual void SetMesh(const Ref<NativeMesh> &mesh) override;
		virtual void SetPipeline(const Ref<NativePipeline> &pipeline) override;
		virtual void DrawIndexed(uint32_t instances, uint32_t count, size_t idxOffset, size_t vtxOffset) override;

		virtual FrameStatistics GetFrameStatistics() const override;

	private:
		void UpdatePipeline();

	private:
		Ref<OpenGLGraphicsContext> mContext;
		Ref<OpenGLPipeline> mPipeline;
		Ref<OpenGLMesh> mMesh;
		Ref<OpenGLFramebuffer> mFramebuffer;
		Ref<OpenGLRenderPass> mRenderPass;

		Rect2D mSpecifiedViewport;
		Rect2D mOutputViewport;

		Rect2D mSpecifiedScissor;
		Rect2D mOutputScissor;

		FrameStatistics mFrameStatistics;
	};

}