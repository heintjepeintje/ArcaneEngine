#pragma once

#include "OpenGL.hpp"
#include <Arcane/Native/NativeBuffer.hpp>

namespace Arcane {

	class OpenGLBuffer : public NativeBuffer {
	public:
		OpenGLBuffer(size_t size, uint32_t flags);
		~OpenGLBuffer();

		virtual void *Map(MapMode mode) override;
		virtual void Unmap() override;
		virtual void SetData(size_t offset, size_t size, const void *data) override;
		virtual void Resize(size_t size) override;

		inline size_t GetSize() const { return mSize; }

		inline GLuint GetOpenGLID() const { return mBuffer; }

	private:
		size_t mSize;
		GLuint mBuffer;
	};

}