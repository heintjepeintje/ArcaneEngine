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
			AR_OPENGL_ASSERT(info.VertexShaderBinary, "Vertex shader binary is invalid");
			
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderBinary(1, &vertexShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.VertexShaderBinary.GetPointer(), info.VertexShaderBinary.GetSize());
			glAttachShader(mProgram, vertexShader);
			glSpecializeShader(vertexShader, "main", 0, nullptr, nullptr);
		}

		if (mStageFlags & ShaderStage::TessellationControl) {
			AR_OPENGL_ASSERT(info.TesselationControlShaderBinary, "Tessellation control shader binary is invalid");

			GLuint tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
			glShaderBinary(1, &tessControlShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.TesselationControlShaderBinary.GetPointer(), info.TesselationControlShaderBinary.GetSize());
			glAttachShader(mProgram, tessControlShader);
			glSpecializeShader(tessControlShader, "main", 0, nullptr, nullptr);
		}

		if (mStageFlags & ShaderStage::TessellationEvaluation) {
			AR_OPENGL_ASSERT(info.TesselationEvaluationShaderBinary, "Tessellation evaluation shader binary is invalid");

			GLuint tessEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
			glShaderBinary(1, &tessEvalShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.TesselationEvaluationShaderBinary.GetPointer(), info.TesselationEvaluationShaderBinary.GetSize());
			glAttachShader(mProgram, tessEvalShader);
			glSpecializeShader(tessEvalShader, "main", 0, nullptr, nullptr);
		}

		if (mStageFlags & ShaderStage::Geometry) {
			AR_OPENGL_ASSERT(info.GeometryShaderBinary, "Geometry shader binary is invalid");

			GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderBinary(1, &geometryShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.GeometryShaderBinary.GetPointer(), info.GeometryShaderBinary.GetSize());
			glAttachShader(mProgram, geometryShader);
			glSpecializeShader(geometryShader, "main", 0, nullptr, nullptr);
		}

		if (mStageFlags & ShaderStage::Fragment) {
			AR_OPENGL_ASSERT(info.FragmentShaderBinary, "Fragment shader binary is invalid");

			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderBinary(1, &fragmentShader, GL_SHADER_BINARY_FORMAT_SPIR_V, info.FragmentShaderBinary.GetPointer(), info.FragmentShaderBinary.GetSize());
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

			AR_OPENGL_ASSERT(false, "Shader Program Linker Error: {}\n", buffer);
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
		Destroy();
	}

	void OpenGLPipeline::SetUniformBuffer(uint32_t binding, const Ref<NativeBuffer> &uniformBuffer, size_t offset, size_t size) {
		AR_PROFILE_FUNCTION();
		
		mUniformBufferDescriptors[binding].binding = binding;
		mUniformBufferDescriptors[binding].buffer = CastRef<OpenGLBuffer>(uniformBuffer)->GetOpenGLID();
		mUniformBufferDescriptors[binding].offset = offset;
		mUniformBufferDescriptors[binding].size = size;
	}

	void OpenGLPipeline::SetCombinedImageSampler(uint32_t binding, const Ref<NativeTexture> &texture, const Ref<NativeSampler> &sampler) {
		AR_PROFILE_FUNCTION();
		mCombinedImageSamplerDescriptors[binding].binding = binding;
		mCombinedImageSamplerDescriptors[binding].texture = CastRef<OpenGLTexture>(texture)->GetOpenGLID();
		mCombinedImageSamplerDescriptors[binding].sampler = CastRef<OpenGLSampler>(sampler)->GetOpenGLID();
	}

	void OpenGLPipeline::Destroy() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		delete[] mUniformBufferDescriptors;
		delete[] mCombinedImageSamplerDescriptors;
		glDeleteProgram(mProgram);
		glDeleteProgramPipelines(1, &mProgramPipeline);
	}

	bool OpenGLPipeline::IsValid() const {
		AR_PROFILE_FUNCTION_GPU_CPU();
		return glIsProgram(mProgram) && glIsProgramPipeline(mProgramPipeline);
	}

}