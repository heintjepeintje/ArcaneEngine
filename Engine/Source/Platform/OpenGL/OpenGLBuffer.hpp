#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeBuffer.hpp>

#include "OpenGLCore.hpp"
#include "OpenGLGraphicsContext.hpp"

namespace Arcane {

	class OpenGLBuffer : public NativeBuffer {
	public:
		OpenGLBuffer(const Ref<OpenGLGraphicsContext> &context, size_t size, uint32_t flags);
		~OpenGLBuffer();

		virtual void *Map(MapMode mode) override;
		virtual void Unmap() override;
		virtual void SetData(size_t offset, size_t size, const void *data) override;
		virtual void Resize(size_t size) override;

		inline size_t GetSize() const { return mSize; }

		inline GLuint GetOpenGLID() const { return mBuffer; }

		virtual void Destroy() override;
		virtual bool IsValid() const override;

	private:
		Ref<OpenGLGraphicsContext> mContext;
		size_t mSize;
		uint32_t mFlags;
		GLuint mBuffer;
	};

}