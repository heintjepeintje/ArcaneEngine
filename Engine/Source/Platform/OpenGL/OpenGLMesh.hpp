#pragma once

#include <Arcane/Native/NativeMesh.hpp>
#include "OpenGLGraphicsContext.hpp"
#include "OpenGLBuffer.hpp"

namespace Arcane {

	class OpenGLMesh : public NativeMesh {
	public:
		OpenGLMesh(const Ref<OpenGLGraphicsContext> &context);
		~OpenGLMesh();

		virtual void SetVertexBuffer(uint32_t index, const InputLayout &layout, const Ref<NativeBuffer> &vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<NativeBuffer> &indexBuffer) override;
		inline virtual uint32_t GetIndexCount() const override { return mIndexBuffer->GetSize() / sizeof(uint32_t); }

		inline virtual InputLayout GetLayout() const override { return mLayout; }	

		inline GLuint GetVertexArray() const { return mVertexArray; }

	private:
		Ref<OpenGLGraphicsContext> mContext;
		GLuint mVertexArray;
		InputLayout mLayout;
		std::vector<Ref<NativeBuffer>> mVertexBuffers;
		Ref<NativeBuffer> mIndexBuffer;
	};

}