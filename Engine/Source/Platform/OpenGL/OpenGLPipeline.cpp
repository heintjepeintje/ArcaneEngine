#include "OpenGLPipeline.hpp"

namespace Arcane {

	OpenGLPipeline::OpenGLPipeline(const std::shared_ptr<OpenGLGraphicsContext> &context, const PipelineInfo &info) : mContext(context) {
		switch (info.CullMode) {
			case CullMode::Back: mCullMode = GL_BACK; break;
			case CullMode::Front: mCullMode = GL_BACK; break;
			case CullMode::FrontAndBack: mCullMode = GL_FRONT_AND_BACK; break;
			default: mCullMode = GL_NONE; break;
		}

		switch (info.WindingOrder) {
			case WindingOrder::Clockwise: mCullMode = GL_CW; break;
			case WindingOrder::CounterClockwise: mCullMode = GL_CCW; break;
			default: mFillMode = GL_NONE; break;
		}

		switch (info.FillMode) {
			case FillMode::Solid: mFillMode = GL_FILL; break;
			case FillMode::Wireframe: mFillMode = GL_LINE; break;
			case FillMode::Points: mFillMode = GL_POINT; break;
			default: mFillMode = GL_NONE; break;
		}

		switch (info.Topology) {
			case PrimitiveTopology::TriangleList: mTopology = GL_TRIANGLES; break;
			case PrimitiveTopology::TriangleStrip: mTopology = GL_TRIANGLE_STRIP; break;
			case PrimitiveTopology::LineList: mTopology = GL_LINES; break;
			case PrimitiveTopology::LineStrip: mTopology = GL_LINE_STRIP; break;
			case PrimitiveTopology::PointList: mTopology = GL_POINTS; break;
			default: mTopology = GL_NONE; break;
		}

		mLayout = info.Layout;
		mViewport.Position.x = Clamp(info.Viewport.Position.x, 0, context->GetWindow()->GetClientSize().x);
		mViewport.Position.y = Clamp(info.Viewport.Position.y, 0, context->GetWindow()->GetClientSize().y);
		mViewport.Size.x = Clamp(info.Viewport.Size.x, 0, context->GetWindow()->GetClientSize().x - mViewport.Position.x);
		mViewport.Size.y = Clamp(info.Viewport.Size.y, 0, context->GetWindow()->GetClientSize().y - mViewport.Position.y);

		mScissor = info.Scissor;
		mScissor.Position.x = Clamp(info.Scissor.Position.x, 0, mViewport.Size.x);
		mScissor.Position.y = Clamp(info.Scissor.Position.y, 0, mViewport.Size.y);
		mScissor.Size.x = Clamp(info.Scissor.Size.x, 0, mViewport.Size.x - mScissor.Position.x);
		mScissor.Size.y = Clamp(info.Scissor.Size.y, 0, mViewport.Size.y - mScissor.Position.y);
		
		mProgram = glCreateProgram();
		
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderBinary(1, &vertexShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.VertexShaderBinary, info.VertexShaderBinarySize);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderBinary(1, &fragmentShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.FragmentShaderBinary, info.FragmentShaderBinarySize);

		glAttachShader(mProgram, vertexShader);
		glAttachShader(mProgram, fragmentShader);

		glLinkProgram(mProgram);

		GLint status = GL_FALSE;
		glGetProgramiv(mProgram, GL_LINK_STATUS, &status);
		if (status != GL_TRUE) {
			GLint length = 0;
			glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &length);

			char *buffer = (char*)alloca(length * sizeof(char));
			glGetProgramInfoLog(mProgram, length, NULL, buffer);

			std::fprintf(stderr, "Error linking program: %s\n", buffer);
			return;
		}

		glValidateProgram(mProgram);
	}

}