#include "OpenGLRenderer.hpp"

#include <Arcane/System/Time.hpp>

namespace Arcane {

	OpenGLRendererAPI::OpenGLRendererAPI(const Ref<OpenGLGraphicsContext> &context) : mContext(context) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glDisable(GL_DITHER);
	}

	OpenGLRendererAPI::~OpenGLRendererAPI() {
		AR_PROFILE_FUNCTION_GPU_CPU();
	}

	void OpenGLRendererAPI::UpdatePipeline() {
		AR_PROFILE_FUNCTION_GPU_CPU();
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

		if (mPipeline->GetPolygonOffsetFactor() != 0.0f || mPipeline->GetPolygonOffsetUnits() != 0.0f) {
			switch (mPipeline->GetFillMode()) {
				case FillMode::Points: glEnable(GL_POLYGON_OFFSET_POINT); break;
				case FillMode::Wireframe: glEnable(GL_POLYGON_OFFSET_LINE); break;
				case FillMode::Solid: glEnable(GL_POLYGON_OFFSET_FILL); break;
			}
			glPolygonOffset(mPipeline->GetPolygonOffsetFactor(), mPipeline->GetPolygonOffsetUnits());
		}

		AR_ASSERT(mFramebuffer->GetSampleCount() == mPipeline->GetSampleCount(), "Framebuffer sample count does not match pipeline sample count: %u != %u", mFramebuffer->GetSampleCount(), mPipeline->GetSampleCount());

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
		AR_PROFILE_FUNCTION_GPU_CPU();
		std::memset(&mFrameStatistics, 0, sizeof(mFrameStatistics));

		mFrameStatistics.ElapsedCPUTime = GetCurrentTimeMillis();
	}

	void OpenGLRendererAPI::End() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glBlitNamedFramebuffer(
			mFramebuffer->GetOpenGLID(), 0,
			0, 0, mFramebuffer->GetWidth(), mFramebuffer->GetHeight(), 
			0, 0, mOutputViewport.Size.X, mOutputViewport.Size.Y, 
			GL_COLOR_BUFFER_BIT,
			GL_LINEAR
		);
		
		// GLsync sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		// glWaitSync(sync, 0, GL_TIMEOUT_IGNORED);
		// glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 0);
		
		mFrameStatistics.ElapsedCPUTime = GetCurrentTimeMillis() - mFrameStatistics.ElapsedCPUTime;
	}

	void OpenGLRendererAPI::BeginRenderPass(const Ref<NativeRenderPass> &renderPass, const Ref<NativeFramebuffer> &framebuffer) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		AR_ASSERT(renderPass->GetAttachmentCount() == framebuffer->GetAttachmentCount(), "RenderPass and Framebuffer attachments are not compatible\n");

		const ImageFormat *renderPassAttachments = renderPass->GetAttachments();
		const ImageFormat *framebufferAttachments = framebuffer->GetAttachments();

		for (size_t i = 0; i < renderPass->GetAttachmentCount(); i++) {
			AR_ASSERT(renderPassAttachments[i] == framebufferAttachments[i], "RenderPass and Framebuffer attachments are not compatible: %u != %u\n", renderPassAttachments[i], framebufferAttachments[i]);
		}

		mFramebuffer = CastRef<OpenGLFramebuffer>(framebuffer);
		mRenderPass = CastRef<OpenGLRenderPass>(renderPass);

		mPipeline = CastRef<OpenGLPipeline>(renderPass->GetPipeline());

		glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer->GetOpenGLID());

		UpdatePipeline();
	}

	void OpenGLRendererAPI::EndRenderPass() {
		AR_PROFILE_FUNCTION_GPU_CPU();
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
		AR_PROFILE_FUNCTION_GPU_CPU();
		glClearColor(r, g, b, a);
	}

	void OpenGLRendererAPI::SetClearDepth(float depth) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glClearDepthf(depth);
	}

	void OpenGLRendererAPI::SetClearStencil(uint16_t stencil) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glClearStencil(stencil);
	}

	void OpenGLRendererAPI::Clear() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		
		GLbitfield clearMask = 0;
		if (mFramebuffer->GetColorAttachmentCount() > 0) clearMask |= GL_COLOR_BUFFER_BIT;
		if (mFramebuffer->HasDepthTexture()) clearMask |= GL_DEPTH_BUFFER_BIT;
		if (mFramebuffer->HasStencilTexture()) clearMask |= GL_STENCIL_BUFFER_BIT;

		glClear(clearMask);
	}

	void OpenGLRendererAPI::SetViewport(Rect2D viewport) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		mSpecifiedViewport = viewport;
	}

	void OpenGLRendererAPI::SetScissor(Rect2D scissor) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		mSpecifiedScissor = scissor;
	}

	void OpenGLRendererAPI::SetMesh(const Ref<NativeMesh> &mesh) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		mMesh = CastRef<OpenGLMesh>(mesh);
		glBindVertexArray(mMesh->GetVertexArray());
	}

	void OpenGLRendererAPI::SetPipeline(const Ref<NativePipeline> &pipeline) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		mPipeline = CastRef<OpenGLPipeline>(pipeline);

		UpdatePipeline();
	}

	void OpenGLRendererAPI::DrawIndexed(uint32_t instances, uint32_t count) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		GLenum topology = GL_NONE;
		switch (mPipeline->GetTopology()) {
			case PrimitiveTopology::TriangleList: topology = GL_TRIANGLES; break;
			case PrimitiveTopology::TriangleStrip: topology = GL_TRIANGLE_STRIP; break;
			case PrimitiveTopology::LineList: topology = GL_LINES; break;
			case PrimitiveTopology::LineStrip: topology = GL_LINE_STRIP; break;
			case PrimitiveTopology::PointList: topology = GL_POINTS; break;
			default: return;
		}

		{
			AR_PROFILE_SCOPE_GPU("Pipeline Buffer Binding");
			for (size_t i = 0; i < mPipeline->GetUniformBufferDescriptorCount(); i++) {
				OpenGLUniformBufferDescriptor &desc = mPipeline->GetUniformBufferDescriptors()[i];
				AR_ASSERT(desc.buffer != 0, "Buffer is invalid in descriptor: %u\n", desc.binding);
				glBindBufferBase(GL_UNIFORM_BUFFER, desc.binding, desc.buffer);
			}
		}

		{
			AR_PROFILE_SCOPE_GPU("Pipeline Combined Image Sampler Binding");
			for (size_t i = 0; i < mPipeline->GetCombinedImageSamplerDescriptorCount(); i++) {
				OpenGLCombinedImageSamplerDescriptor &desc = mPipeline->GetCombinedImageSamplerDescriptors()[i];
				AR_ASSERT(desc.texture != 0, "Texture is invalid in descriptor: %u\n", desc.binding);
				AR_ASSERT(desc.sampler != 0, "Sampler is invalid in descriptor: %u\n", desc.binding);
				glBindTextureUnit(desc.binding, desc.texture);
				glBindSampler(desc.binding, desc.sampler);	
			}
		}

		{
			AR_PROFILE_SCOPE_GPU("Draw Call");
			glDrawElementsInstanced(topology, count, mPipeline->GetElementSize() == 4 ? GL_UNSIGNED_INT : GL_UNSIGNED_BYTE, nullptr, instances);
		}

		mFrameStatistics.DrawCommands++;
		mFrameStatistics.IndicesDrawn += count * instances;
		mFrameStatistics.InstancesDrawn += instances;
	}

	FrameStatistics OpenGLRendererAPI::GetFrameStatistics() const {
		return mFrameStatistics;
	}

}