#pragma once

#include <Arcane/Native/NativeMesh.hpp>
#include "OpenGLGraphicsContext.hpp"
#include "OpenGLBuffer.hpp"

namespace Arcane {

	class OpenGLMesh : public NativeMesh {
	public:
		OpenGLMesh(const std::shared_ptr<OpenGLGraphicsContext> &context);
		~OpenGLMesh();

		virtual void SetVertexBuffer(uint32_t index, const InputLayout &layout, const std::shared_ptr<NativeBuffer> &vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<NativeBuffer> &indexBuffer) override;

		inline virtual InputLayout GetLayout() const override { return mLayout; }	
	private:
		std::shared_ptr<OpenGLGraphicsContext> mContext;
		GLuint mVertexArray;
		InputLayout mLayout;
		std::vector<std::shared_ptr<NativeBuffer>> mVertexBuffers;
		std::shared_ptr<NativeBuffer> mIndexBuffer;
	};

}