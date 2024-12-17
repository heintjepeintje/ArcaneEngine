#include "OpenGLRenderer.hpp"

namespace Arcane {

	OpenGLRendererAPI::OpenGLRendererAPI(const std::shared_ptr<OpenGLGraphicsContext> &context) : mContext(context) {
		glEnable(GL_SCISSOR_TEST);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}

	OpenGLRendererAPI::~OpenGLRendererAPI() { }

	void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetViewport(Rect2D viewport) {
		mViewport = viewport;
	}

	void OpenGLRendererAPI::SetScissor(Rect2D scissor) {
		mScissor = scissor;
	}

	void OpenGLRendererAPI::SetPipeline(const std::shared_ptr<NativePipeline> &pipeline) {
		mPipeline = std::dynamic_pointer_cast<OpenGLPipeline>(pipeline);

		glEnable(GL_CULL_FACE);

		switch (pipeline->GetCullMode()) {
			case CullMode::Back: glCullFace(GL_BACK); break;
			case CullMode::Front: glCullFace(GL_FRONT); break;
			case CullMode::FrontAndBack: glCullFace(GL_FRONT_AND_BACK); break;
			default: glDisable(GL_CULL_FACE); break;
		}

		switch (pipeline->GetWindingOrder()) {
			case WindingOrder::Clockwise: glFrontFace(GL_CW); break;
			case WindingOrder::CounterClockwise: glFrontFace(GL_CCW); break;
		}

		switch (pipeline->GetFillMode()) {
			case FillMode::Solid: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
			case FillMode::Wireframe: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
			case FillMode::Points: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
			default: glPolygonMode(GL_FRONT_AND_BACK, GL_NONE); break;
		}
	}

	void OpenGLRendererAPI::SetMesh(const std::shared_ptr<NativeMesh> &mesh) {
		mMesh = std::dynamic_pointer_cast<OpenGLMesh>(mesh);
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

		Rect2D viewport;
		if (mPipeline->GetViewport().Position == Vector2::Zero() && mPipeline->GetViewport().Size == Vector2::MaxValue()) {
			viewport = mViewport;
		} else {
			viewport = mPipeline->GetViewport();
		}

		glViewport(
			viewport.Position.x, viewport.Position.y,
			viewport.Size.x, viewport.Size.y
		);

		Rect2D scissor;
		if (mPipeline->GetScissor().Position == Vector2::Zero() && mPipeline->GetScissor().Size == Vector2::MaxValue()) {
			scissor = mScissor;
		} else {
			scissor = mPipeline->GetScissor();
		}
		
		glScissor(
			scissor.Position.x, scissor.Position.y,
			scissor.Size.x, scissor.Size.y
		);

		glBindVertexArray(mMesh->GetVertexArray());
		glUseProgram(mPipeline->GetShaderProgram());

		for (OpenGLUniformBufferDescriptor &uniformBufferDesc : mPipeline->GetUniformBufferDescriptors()) {
			glBindBufferBase(GL_UNIFORM_BUFFER, uniformBufferDesc.binding, uniformBufferDesc.buffer);
		}

		glDrawElements(topology, count, GL_UNSIGNED_INT, nullptr);
		glUseProgram(0);
		glBindVertexArray(0);
	}

	std::shared_ptr<NativeRendererAPI> NativeRendererAPI::Create(const std::shared_ptr<NativeGraphicsContext> &context) {
		return std::make_shared<OpenGLRendererAPI>(
			std::dynamic_pointer_cast<OpenGLGraphicsContext>(context)
		);
	}

}