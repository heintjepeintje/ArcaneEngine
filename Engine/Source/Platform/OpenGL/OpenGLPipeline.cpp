#include "OpenGLPipeline.hpp"

#include "OpenGLBuffer.hpp"
#include "OpenGLTexture.hpp"

namespace Arcane {

	OpenGLPipeline::OpenGLPipeline(const Ref<OpenGLGraphicsContext> &context, const PipelineInfo &info) : mContext(context) {
		mCullMode = info.CullMode;
		mWindingOrder = info.WindingOrder;
		mFillMode = info.FillMode;
		mTopology = info.Topology;
		mLayout = info.Layout;
		mOutputMask = info.OutputMask;

		mViewport = info.Viewport;
		mScissor = info.Scissor;

		mDescriptorCount = info.DescriptorCount;
		mDescriptors = new Descriptor[mDescriptorCount];
		for (uint32_t i = 0; i < mDescriptorCount; i++) {
			mDescriptors[i] = info.Descriptors[i];
		}

		mSampleCount = info.SampleCount;

		mLineWidth = info.LineWidth;
		mPointSize = info.PointSize;

		mElementSize = info.ElementSize;

		mProgram = glCreateProgram();
		
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderBinary(1, &vertexShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.VertexShaderBinary, info.VertexShaderSize);
		glAttachShader(mProgram, vertexShader);
		glSpecializeShader(vertexShader, "main", 0, nullptr, nullptr);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderBinary(1, &fragmentShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.FragmentShaderBinary, info.FragmentShaderSize);
		glAttachShader(mProgram, fragmentShader);
		glSpecializeShader(fragmentShader, "main", 0, nullptr, nullptr);

		glLinkProgram(mProgram);

		GLint status = 0;
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

	void OpenGLPipeline::SetUniformBuffer(uint32_t binding, const Ref<NativeBuffer> &uniformBuffer) {
		mUniformBufferDescriptors.reserve(binding + 1);
		OpenGLUniformBufferDescriptor desc{};
		desc.binding = binding;
		desc.buffer = CastRef<OpenGLBuffer>(uniformBuffer)->GetOpenGLID();
		
		mUniformBufferDescriptors[binding] = desc;
		
		printf("Uniform Buffer: %u: {b: %u, b: %u}\n", binding, desc.binding, desc.buffer);
	}

	void OpenGLPipeline::SetCombinedImageSampler(uint32_t binding, const Ref<NativeTexture> &texture, const Ref<NativeSampler> &sampler) {
		mCombinedImageSamplerDescriptors.reserve(binding + 1);
		OpenGLCombinedImageSamplerDescriptor desc{};
		desc.binding = binding;
		desc.texture = CastRef<OpenGLTexture>(texture)->GetOpenGLID();
		desc.sampler = CastRef<OpenGLSampler>(sampler)->GetOpenGLID(); 
		
		mCombinedImageSamplerDescriptors[binding] = desc;

		printf("Combined Image Sampler: %u: {b: %u, t: %u, s: %u}\n", binding, desc.binding, desc.texture, desc.sampler);
	}

}