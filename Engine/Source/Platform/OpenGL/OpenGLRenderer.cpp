#include "OpenGLRenderer.hpp"

namespace Arcane {

	OpenGLRendererAPI::OpenGLRendererAPI(const Ref<OpenGLGraphicsContext> &context) : mContext(context) {
		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST);
		glClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE);
	}

	OpenGLRendererAPI::~OpenGLRendererAPI() { }

	void OpenGLRendererAPI::Begin() {
		
	}

	void OpenGLRendererAPI::End() {
		glBlitNamedFramebuffer(
			mFramebuffer->GetOpenGLID(), 0,
			0, 0, mFramebuffer->GetWidth(), mFramebuffer->GetHeight(), 
			0, mOutputViewport.Size.Y, mOutputViewport.Size.X, 0, 
			GL_COLOR_BUFFER_BIT, 
			GL_NEAREST
		);
		
		GLsync sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		glWaitSync(sync, 0, GL_TIMEOUT_IGNORED);
	}

	void OpenGLRendererAPI::BeginRenderPass(const Ref<NativeRenderPass> &renderPass, const Ref<NativeFramebuffer> &framebuffer) {
		AR_ASSERT(renderPass->GetAttachmentCount() == framebuffer->GetAttachmentCount(), "RenderPass and Framebuffer attachments are not compatible (count mismatch)");

		const Attachment *renderPassAttachments = renderPass->GetAttachments();
		const Attachment *framebufferAttachments = framebuffer->GetAttachments();

		for (size_t i = 0; i < renderPass->GetAttachmentCount(); i++) {
			AR_ASSERT(renderPassAttachments[i].Format == framebufferAttachments[i].Format, "RenderPass and Framebuffer attachments are not compatible (format mismatch)");
			AR_ASSERT(renderPassAttachments[i].Type == framebufferAttachments[i].Type, "RenderPass and Framebuffer attachments are not compatible (type mismatch)");
		}

		mFramebuffer = CastRef<OpenGLFramebuffer>(framebuffer);
		mRenderPass = CastRef<OpenGLRenderPass>(renderPass);

		mPipeline = CastRef<OpenGLPipeline>(renderPass->GetPipeline());

		glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer->GetOpenGLID());

		glEnable(GL_CULL_FACE);

		switch (mPipeline->GetCullMode()) {
			case CullMode::Back: glCullFace(GL_BACK); break;
			case CullMode::Front: glCullFace(GL_FRONT); break;
			case CullMode::FrontAndBack: glCullFace(GL_FRONT_AND_BACK); break;
			default: glDisable(GL_CULL_FACE); break;
		}

		switch (mPipeline->GetWindingOrder()) {
			case WindingOrder::Clockwise: glFrontFace(GL_CW); break;
			case WindingOrder::CounterClockwise: glFrontFace(GL_CCW); break;
		}

		switch (mPipeline->GetFillMode()) {
			case FillMode::Solid: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
			case FillMode::Wireframe: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
			case FillMode::Points: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
			default: glPolygonMode(GL_FRONT_AND_BACK, GL_NONE); break;
		}

		for (uint32_t i = 0; i < mFramebuffer->GetAttachmentCount(); i++) {
			const Attachment &attachment = mFramebuffer->GetAttachments()[i];
			if (attachment.Type == AttachmentType::Color) {
				AR_ASSERT(attachment.Samples == mPipeline->GetSampleCount(), "Color attachment samples do not match pipeline samples");
			}
		}

		if (mPipeline->GetSampleCount() > 1) {
			glEnable(GL_MULTISAMPLE);
		} else {
			glDisable(GL_MULTISAMPLE);
		}

		glUseProgram(mPipeline->GetShaderProgram());

		for (OpenGLUniformBufferDescriptor &uniformBufferDesc : mPipeline->GetUniformBufferDescriptors()) {
			glBindBufferBase(GL_UNIFORM_BUFFER, uniformBufferDesc.binding, uniformBufferDesc.buffer);
		}

		for (OpenGLCombinedImageSamplerDescriptor &combinedImageSamplerDesc : mPipeline->GetCombinedImageSamplerDescriptors()) {
			glBindTextureUnit(combinedImageSamplerDesc.binding, combinedImageSamplerDesc.texture);
			glBindSampler(combinedImageSamplerDesc.binding, combinedImageSamplerDesc.sampler);
		}

	}

	void OpenGLRendererAPI::EndRenderPass() {
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer->GetOpenGLID());
		for (OpenGLCombinedImageSamplerDescriptor &combinedImageSamplerDesc : mPipeline->GetCombinedImageSamplerDescriptors()) {
			glBindTextureUnit(combinedImageSamplerDesc.binding, 0);
			glBindSampler(combinedImageSamplerDesc.binding, 0);
		}

		for (OpenGLUniformBufferDescriptor &uniformBufferDesc : mPipeline->GetUniformBufferDescriptors()) {
			glBindBufferBase(GL_UNIFORM_BUFFER, uniformBufferDesc.binding, 0);
		}

		glUseProgram(0);

		glBindVertexArray(0);
	}

	void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetViewport(Rect2D viewport) {
		mSpecifiedViewport = viewport;
	}

	void OpenGLRendererAPI::SetScissor(Rect2D scissor) {
		mSpecifiedScissor = scissor;
	}

	void OpenGLRendererAPI::SetMesh(const Ref<NativeMesh> &mesh) {
		mMesh = CastRef<OpenGLMesh>(mesh);
		glBindVertexArray(mMesh->GetVertexArray());
	}

	void OpenGLRendererAPI::DrawIndexed(uint32_t count) {
		GLenum topology = GL_NONE;
		switch (mPipeline->GetTopology()) {
			case PrimitiveTopology::TriangleList: topology = GL_TRIANGLES; break;
			case PrimitiveTopology::TriangleStrip: topology = GL_TRIANGLE_STRIP; break;
			case PrimitiveTopology::LineList: topology = GL_LINES; break;
			case PrimitiveTopology::LineStrip: topology = GL_LINE_STRIP; break;
			case PrimitiveTopology::PointList: topology = GL_POINTS; break;
			default: return;
		}

		if (mPipeline->GetViewport().Position == Vector2(0) && mPipeline->GetViewport().Size == Vector2(__FLT_MAX__)) {
			mOutputViewport = mSpecifiedViewport;
		} else {
			mOutputViewport = mPipeline->GetViewport();
		}

		glViewport(
			mOutputViewport.Position.X, mOutputViewport.Position.Y,
			mOutputViewport.Size.X, mOutputViewport.Size.Y
		);

		if (mPipeline->GetScissor().Position == Vector2(0) && mPipeline->GetScissor().Size == Vector2(__FLT_MAX__)) {
			mOutputScissor = mSpecifiedScissor;
		} else {
			mOutputScissor = mPipeline->GetScissor();
		}
		
		glScissor(
			mOutputScissor.Position.X, mOutputScissor.Position.Y,
			mOutputScissor.Size.X, mOutputScissor.Size.Y
		);

		glDrawElements(topology, count, GL_UNSIGNED_INT, nullptr);
	}

}