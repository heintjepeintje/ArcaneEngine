#pragma once

#include "OpenGLCore.hpp"
#include <Arcane/Native/NativeBuffer.hpp>

namespace Arcane {

	class OpenGLBuffer : public NativeBuffer{
	public:
		OpenGLBuffer(BufferType type, size_t size);
		~OpenGLBuffer();

		virtual void *Map(MapMode mode) override;
		virtual void Unmap() override;

		inline size_t GetSize() const { return mSize; }
		inline virtual BufferType GetType() const { return mType; }

	private:
		BufferType mType;
		size_t mSize;
		GLuint mBuffer;
	};

}