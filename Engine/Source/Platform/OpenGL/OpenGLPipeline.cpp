#include "OpenGLPipeline.hpp"

#include "OpenGLBuffer.hpp"
#include "OpenGLTexture.hpp"

namespace Arcane {

	OpenGLPipeline::OpenGLPipeline(const Ref<OpenGLGraphicsContext> &context, const PipelineInfo &info) : mContext(context) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		mCullMode = info.CullMode;
		mWindingOrder = info.WindingOrder;
		mFillMode = info.FillMode;
		mTopology = info.Topology;
		mLayout = info.Layout;
		mOutputMask = info.OutputMask;

		mViewport = info.Viewport;
		mScissor = info.Scissor;

		mStageFlags = info.StageFlags;

		mUniformBufferDescriptorCount = 0;
		mCombinedImageSamplerDescriptorCount = 0;

		mDescriptorCount = info.DescriptorCount;
		mDescriptors = new Descriptor[mDescriptorCount];
		for (uint32_t i = 0; i < mDescriptorCount; i++) {
			mDescriptors[i] = info.Descriptors[i];
			switch (info.Descriptors[i].Type) {
				case DescriptorType::UniformBuffer: mUniformBufferDescriptorCount++; break;
				case DescriptorType::CombinedImageSampler: mCombinedImageSamplerDescriptorCount++; break;
			}
		}

		mUniformBufferDescriptors = new OpenGLUniformBufferDescriptor[mUniformBufferDescriptorCount];
		mCombinedImageSamplerDescriptors = new OpenGLCombinedImageSamplerDescriptor[mCombinedImageSamplerDescriptorCount];

		mSampleCount = info.SampleCount;

		mLineWidth = info.LineWidth;
		mPointSize = info.PointSize;

		mElementSize = info.ElementSize;

		mProgram = glCreateProgram();
		glProgramParameteri(mProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);

		if (mStageFlags & ShaderStage::Vertex) {
			AR_OPENGL_ASSERT(info.VertexShader.Size > 0, "Vertex shader binary data is empty");
			AR_OPENGL_ASSERT(info.VertexShader.Data != nullptr, "Vertex shader binary data is nullptr");
			
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderBinary(1, &vertexShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.VertexShader.Data, info.VertexShader.Size);
			glAttachShader(mProgram, vertexShader);
			glSpecializeShader(vertexShader, "main", 0, nullptr, nullptr);
		}

		if (mStageFlags & ShaderStage::TessellationControl) {
			AR_OPENGL_ASSERT(info.TesselationControlShader.Size > 0, "Tessellation control shader binary data is empty");
			AR_OPENGL_ASSERT(info.TesselationControlShader.Data != nullptr, "Tessellation control shader binary data is nullptr");

			GLuint tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderBinary(1, &tessControlShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.TesselationControlShader.Data, info.TesselationControlShader.Size);
			glAttachShader(mProgram, tessControlShader);
			glSpecializeShader(tessControlShader, "main", 0, nullptr, nullptr);
		}

		if (mStageFlags & ShaderStage::TessellationEvaluation) {
			AR_OPENGL_ASSERT(info.TesselationEvaluationShader.Size > 0, "Tessellation evaluation shader binary data is empty");
			AR_OPENGL_ASSERT(info.TesselationEvaluationShader.Data != nullptr, "Tessellation evaluation shader binary data is nullptr");

			GLuint tessEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderBinary(1, &tessEvalShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.TesselationEvaluationShader.Data, info.TesselationEvaluationShader.Size);
			glAttachShader(mProgram, tessEvalShader);
			glSpecializeShader(tessEvalShader, "main", 0, nullptr, nullptr);
		}

		if (mStageFlags & ShaderStage::Geometry) {
			AR_OPENGL_ASSERT(info.GeometryShader.Size > 0, "Geometry shader binary data is empty");
			AR_OPENGL_ASSERT(info.GeometryShader.Data != nullptr, "Geometry shader binary data is nullptr");

			GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderBinary(1, &geometryShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.GeometryShader.Data, info.GeometryShader.Size);
			glAttachShader(mProgram, geometryShader);
			glSpecializeShader(geometryShader, "main", 0, nullptr, nullptr);
		}

		if (mStageFlags & ShaderStage::Fragment) {
			AR_OPENGL_ASSERT(info.FragmentShader.Size > 0, "Fragment shader binary data is empty");
			AR_OPENGL_ASSERT(info.FragmentShader.Data != nullptr, "Fragment shader binary data is nullptr");

			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderBinary(1, &fragmentShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.FragmentShader.Data, info.FragmentShader.Size);
			glAttachShader(mProgram, fragmentShader);
			glSpecializeShader(fragmentShader, "main", 0, nullptr, nullptr);
		}

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

		glCreateProgramPipelines(1, &mProgramPipeline);

		GLbitfield stages = 0;
		if (mStageFlags & ShaderStage::Vertex) stages |= GL_VERTEX_SHADER_BIT;
		if (mStageFlags & ShaderStage::TessellationControl) stages |= GL_TESS_CONTROL_SHADER_BIT;
		if (mStageFlags & ShaderStage::TessellationEvaluation) stages |= GL_TESS_EVALUATION_SHADER_BIT;
		if (mStageFlags & ShaderStage::Geometry) stages |= GL_GEOMETRY_SHADER_BIT;
		if (mStageFlags & ShaderStage::Fragment) stages |= GL_FRAGMENT_SHADER_BIT;

		glUseProgramStages(mProgramPipeline, stages, mProgram);
		glValidateProgramPipeline(mProgramPipeline);
	}

	OpenGLPipeline::~OpenGLPipeline() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glDeleteProgram(mProgram);
		glDeleteProgramPipelines(1, &mProgramPipeline);
		delete[] mDescriptors;
	}

	void OpenGLPipeline::SetUniformBuffer(uint32_t binding, const Ref<NativeBuffer> &uniformBuffer) {
		AR_PROFILE_FUNCTION();
		mUniformBufferDescriptors[binding].binding = binding;
		mUniformBufferDescriptors[binding].buffer = CastRef<OpenGLBuffer>(uniformBuffer)->GetOpenGLID();
	}

	void OpenGLPipeline::SetCombinedImageSampler(uint32_t binding, const Ref<NativeTexture> &texture, const Ref<NativeSampler> &sampler) {
		AR_PROFILE_FUNCTION();
		mCombinedImageSamplerDescriptors[binding].binding = binding;
		mCombinedImageSamplerDescriptors[binding].texture = CastRef<OpenGLTexture>(texture)->GetOpenGLID();
		mCombinedImageSamplerDescriptors[binding].sampler = CastRef<OpenGLSampler>(sampler)->GetOpenGLID();
	}

}