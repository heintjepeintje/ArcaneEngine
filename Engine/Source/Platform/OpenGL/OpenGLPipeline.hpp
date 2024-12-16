#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativePipeline.hpp>
#include "OpenGLCore.hpp"
#include "OpenGLGraphicsContext.hpp"

namespace Arcane {

	class OpenGLPipeline : public NativePipeline {
	public:
		OpenGLPipeline(const std::shared_ptr<OpenGLGraphicsContext> &context, const PipelineInfo &info);
		~OpenGLPipeline();

	private:
		std::shared_ptr<OpenGLGraphicsContext> mContext;
		GLuint mProgram;
		
		GLenum mCullMode;
		GLenum mWindingOrder;
		GLenum mFillMode;
		GLenum mTopology;
		InputLayout mLayout;
		Rect2D mViewport;
		Rect2D mScissor;
	};

}