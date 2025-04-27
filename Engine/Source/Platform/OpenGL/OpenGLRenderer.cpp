#include "OpenGLRenderer.hpp"

#include <Arcane/System/Time.hpp>

namespace Arcane {

	OpenGLRendererAPI::OpenGLRendererAPI(const Ref<OpenGLGraphicsContext> &context) : mContext(context) {
		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
	}

	OpenGLRendererAPI::~OpenGLRendererAPI() { }

	void OpenGLRendererAPI::UpdatePipeline() {
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

		const uint8_t outputMask = mPipeline->GetOutputMask();
		glColorMask(
			outputMask & (uint8_t)OutputComponent::Red,
			outputMask & (uint8_t)OutputComponent::Green,
			outputMask & (uint8_t)OutputComponent::Blue,
			outputMask & (uint8_t)OutputComponent::Alpha
		);
		glDepthMask(outputMask & (uint8_t)OutputComponent::Depth);
		glStencilMask(outputMask & (uint8_t)OutputComponent::Stencil);

		for (uint32_t i = 0; i < mFramebuffer->GetAttachmentCount(); i++) {
			AR_ASSERT(
				mFramebuffer->GetAttachments()[i].Samples == mPipeline->GetSampleCount(), 
				"Attachment samples does not match pipeline samples"
			);
		}

		GLenum *drawBuffers = new GLenum[mFramebuffer->GetColorBufferCount()];
		for (uint32_t i = 0; i < mFramebuffer->GetColorBufferCount(); i++) {
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}

		glDrawBuffers(mFramebuffer->GetColorBufferCount(), drawBuffers);

		if (mPipeline->GetSampleCount() > 1) {
			glEnable(GL_MULTISAMPLE);
		} else {
			glDisable(GL_MULTISAMPLE);
		}

		glUseProgram(mPipeline->GetShaderProgram());

		if (mPipeline->GetViewport().Position == Vector2::Zero() && mPipeline->GetViewport().Size == Vector2::MaxValue()) {
			mOutputViewport = mSpecifiedViewport;
		} else {
			mOutputViewport = mPipeline->GetViewport();
		}

		glViewport(
			mOutputViewport.Position.X, mOutputViewport.Position.Y,
			mOutputViewport.Size.X, mOutputViewport.Size.Y
		);

		if (mPipeline->GetScissor().Position == Vector2::Zero() && mPipeline->GetScissor().Size == Vector2::MaxValue()) {
			mOutputScissor = mSpecifiedScissor;
		} else {
			mOutputScissor = mPipeline->GetScissor();
		}
		
		glScissor(
			mOutputScissor.Position.X, mOutputScissor.Position.Y,
			mOutputScissor.Size.X, mOutputScissor.Size.Y
		);

		glLineWidth(mPipeline->GetLineWidth());
		glPointSize(mPipeline->GetPointSize());
	}

	void OpenGLRendererAPI::Begin() {
		std::memset(&mFrameStatistics, 0, sizeof(mFrameStatistics));

		mFrameStatistics.ElapsedCPUTime = GetCurrentTimeMillis();
	}

	void OpenGLRendererAPI::End() {
		glBlitNamedFramebuffer(
			mFramebuffer->GetOpenGLID(), 0,
			0, 0, mFramebuffer->GetWidth(), mFramebuffer->GetHeight(), 
			0, 0, mOutputViewport.Size.X, mOutputViewport.Size.Y, 
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR
		);
		
		GLsync sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		glWaitSync(sync, 0, GL_TIMEOUT_IGNORED);

		
		mFrameStatistics.ElapsedCPUTime = GetCurrentTimeMillis() - mFrameStatistics.ElapsedCPUTime;
	}

	void OpenGLRendererAPI::BeginRenderPass(const Ref<NativeRenderPass> &renderPass, const Ref<NativeFramebuffer> &framebuffer) {
		AR_ASSERT(renderPass->GetAttachmentCount() == framebuffer->GetAttachmentCount(), "RenderPass and Framebuffer attachments are not compatible");

		const Attachment *renderPassAttachments = renderPass->GetAttachments();
		const Attachment *framebufferAttachments = framebuffer->GetAttachments();

		for (size_t i = 0; i < renderPass->GetAttachmentCount(); i++) {
			AR_ASSERT(renderPassAttachments[i].Format == framebufferAttachments[i].Format, "RenderPass and Framebuffer attachments are not compatible");
			AR_ASSERT(renderPassAttachments[i].Type == framebufferAttachments[i].Type, "RenderPass and Framebuffer attachments are not compatible");
		}

		mFramebuffer = CastRef<OpenGLFramebuffer>(framebuffer);
		mRenderPass = CastRef<OpenGLRenderPass>(renderPass);

		mPipeline = CastRef<OpenGLPipeline>(renderPass->GetPipeline());

		glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer->GetOpenGLID());

		UpdatePipeline();
	}

	void OpenGLRendererAPI::EndRenderPass() {
		glBindVertexArray(0);
		
		for (size_t i = 0; i < mPipeline->GetCombinedImageSamplerDescriptorCount(); i++) {
			OpenGLCombinedImageSamplerDescriptor &desc = mPipeline->GetCombinedImageSamplerDescriptors()[i];
			if (i != desc.binding) continue;
			glBindTextureUnit(desc.binding, 0);
			glBindSampler(desc.binding, 0);
		}

		for (size_t i = 0; i < mPipeline->GetUniformBufferDescriptorCount(); i++) {
			OpenGLUniformBufferDescriptor &desc = mPipeline->GetUniformBufferDescriptors()[i];
			if (i != desc.binding) continue;
			glBindBufferBase(GL_UNIFORM_BUFFER, desc.binding, 0);
		}

	}

	void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void OpenGLRendererAPI::SetClearDepth(float depth) {
		glClearDepthf(depth);
	}

	void OpenGLRendererAPI::SetClearStencil(uint16_t stencil) {
		glClearStencil(stencil);
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

	void OpenGLRendererAPI::SetPipeline(const Ref<NativePipeline> &pipeline) {
		mPipeline = CastRef<OpenGLPipeline>(pipeline);

		UpdatePipeline();
	}

	void OpenGLRendererAPI::DrawIndexed(uint32_t instances, uint32_t count) {
		GLenum topology = GL_NONE;
		switch (mPipeline->GetTopology()) {
			case PrimitiveTopology::TriangleList: topology = GL_TRIANGLES; break;
			case PrimitiveTopology::TriangleStrip: topology = GL_TRIANGLE_STRIP; break;
			case PrimitiveTopology::LineList: topology = GL_LINES; break;
			case PrimitiveTopology::LineStrip: topology = GL_LINE_STRIP; break;
			case PrimitiveTopology::PointList: topology = GL_POINTS; break;
			default: return;
		}

		for (size_t i = 0; i < mPipeline->GetUniformBufferDescriptorCount(); i++) {
			OpenGLUniformBufferDescriptor &desc = mPipeline->GetUniformBufferDescriptors()[i];
			glBindBufferBase(GL_UNIFORM_BUFFER, desc.binding, desc.buffer);
		}

		for (size_t i = 0; i < mPipeline->GetCombinedImageSamplerDescriptorCount(); i++) {
			OpenGLCombinedImageSamplerDescriptor &desc = mPipeline->GetCombinedImageSamplerDescriptors()[i];
			glBindTextureUnit(desc.binding, desc.texture);
			glBindSampler(desc.binding, desc.sampler);
		}

		glDrawElementsInstanced(topology, count, mPipeline->GetElementSize() == 4 ? GL_UNSIGNED_INT : GL_UNSIGNED_BYTE, nullptr, instances);

		mFrameStatistics.DrawCommands++;
		mFrameStatistics.IndicesDrawn += count * instances;
		mFrameStatistics.InstancesDrawn += instances;
	}

	FrameStatistics OpenGLRendererAPI::GetFrameStatistics() const {
		return mFrameStatistics;
	}

}