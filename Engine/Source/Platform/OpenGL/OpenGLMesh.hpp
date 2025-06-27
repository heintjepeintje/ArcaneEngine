#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeMesh.hpp>

#include "OpenGLCore.hpp"
#include "OpenGLGraphicsContext.hpp"
#include "OpenGLBuffer.hpp"

namespace Arcane {

	class OpenGLMesh : public NativeMesh {
	public:
		OpenGLMesh(const Ref<OpenGLGraphicsContext> &context);
		~OpenGLMesh();

		virtual void SetVertexBuffer(uint32_t index, const InputLayout &layout, const Ref<NativeBuffer> &vertexBuffer, size_t offset, size_t stride) override;
		virtual void SetIndexBuffer(const Ref<NativeBuffer> &indexBuffer) override;
		virtual Ref<NativeBuffer> GetVertexBuffer(uint32_t index) override;
		virtual Ref<NativeBuffer> GetIndexBuffer() override;

		inline virtual InputLayout GetLayout() const override { return mLayout; }	

		inline GLuint GetVertexArray() const { return mVertexArray; }

		virtual void Destroy() override;
		virtual bool IsValid() const override;

	private:
		Ref<OpenGLGraphicsContext> mContext;
		GLuint mVertexArray;
		InputLayout mLayout;
		std::vector<Ref<NativeBuffer>> mVertexBuffers;
		Ref<NativeBuffer> mIndexBuffer;
	};

}