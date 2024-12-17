#include "OpenGLPipeline.hpp"

#include "OpenGLBuffer.hpp"

namespace Arcane {

	OpenGLPipeline::OpenGLPipeline(const std::shared_ptr<OpenGLGraphicsContext> &context, const PipelineInfo &info) : mContext(context) {
		mCullMode = info.CullMode;
		mWindingOrder = info.WindingOrder;
		mFillMode = info.FillMode;
		mTopology = info.Topology;
		mLayout = info.Layout;

		mViewport = info.Viewport;
		mScissor = info.Scissor;

		mDescriptorCount = info.DescriptorCount;
		mDescriptors = new Descriptor[mDescriptorCount];
		for (uint32_t i = 0; i < mDescriptorCount; i++) {
			mDescriptors[i] = info.Descriptors[i];
		}

		mProgram = glCreateProgram();
		
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		GLint vertexShaderLengths[] = { (GLint)info.VertexShaderSourceLength };

		glShaderSource(vertexShader, 1, &info.VertexShaderSource, vertexShaderLengths);
		glCompileShader(vertexShader);

		GLint status = GL_FALSE;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			GLint length = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);

			char *buffer = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(vertexShader, length, NULL, buffer);

			AR_ASSERT(false, "Vertex Shader Compilation Error: %s\n", buffer);
			return;
		}

		GLint fragmentShaderLengths[] = { (GLint)info.FragmentShaderSourceLength };

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &info.FragmentShaderSource, fragmentShaderLengths);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			GLint length = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);

			char *buffer = (char*)alloca(length * sizeof(char));
			glGetShaderInfoLog(fragmentShader, length, NULL, buffer);

			AR_ASSERT(false, "Framgent Shader Compilation Error: %s\n", buffer);
			return;
		}

		glAttachShader(mProgram, vertexShader);
		glAttachShader(mProgram, fragmentShader);

		glLinkProgram(mProgram);

		glGetProgramiv(mProgram, GL_LINK_STATUS, &status);
		if (status != GL_TRUE) {
			GLint length = 0;
			glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &length);

			char *buffer = (char*)alloca(length * sizeof(char));
			glGetProgramInfoLog(mProgram, length, NULL, buffer);

			AR_ASSERT(false, "Shader Program Linker Error: %s\n", buffer);
			return;
		}

		glValidateProgram(mProgram);
	}

	OpenGLPipeline::~OpenGLPipeline() {
		glDeleteProgram(mProgram);
		delete[] mDescriptors;
	}

	void OpenGLPipeline::SetDescriptor(uint32_t binding, const std::shared_ptr<NativeBuffer> &uniformBuffer) {
		mUniformBufferDescriptors.push_back({ binding, std::dynamic_pointer_cast<OpenGLBuffer>(uniformBuffer)->GetOpenGLID() });
	}

	std::shared_ptr<NativePipeline> NativePipeline::Create(const std::shared_ptr<NativeGraphicsContext> &context, const PipelineInfo &info) {
		return std::make_shared<OpenGLPipeline>(
			std::dynamic_pointer_cast<OpenGLGraphicsContext>(context),
			info
		);
	}

}