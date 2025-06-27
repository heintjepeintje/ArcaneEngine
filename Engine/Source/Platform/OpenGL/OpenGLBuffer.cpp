#include "OpenGLBuffer.hpp"

namespace Arcane {

	OpenGLBuffer::OpenGLBuffer(const Ref<OpenGLGraphicsContext> &context, size_t size, uint32_t flags) : mContext(context), mSize(size) {
		AR_PROFILE_FUNCTION_GPU_CPU();

		mFlags = flags;
		mSize = size;

		glCreateBuffers(1, &mBuffer);
		if (mFlags & BufferFlag::Mutable) {
			glNamedBufferData(mBuffer, mSize, nullptr, flags & BufferFlag::Static ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
		} else {
			AR_ASSERT(size != 0, "Size of immutable buffer cannot be 0\n");
			
			GLbitfield glFlags = 0;
			if (flags & BufferFlag::MapRead) glFlags |= GL_MAP_READ_BIT;
			if (flags & BufferFlag::MapWrite) glFlags |= GL_MAP_WRITE_BIT;
			
			glNamedBufferStorage(mBuffer, mSize, nullptr, GL_DYNAMIC_STORAGE_BIT | glFlags);
		}
	}

	OpenGLBuffer::~OpenGLBuffer() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		Destroy();
	}

	void *OpenGLBuffer::Map(MapMode mode) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		GLenum access = 0;
		switch (mode) {
			case MapMode::Read: access = GL_READ_ONLY; break;
			case MapMode::Write: access = GL_WRITE_ONLY; break;
			case MapMode::ReadWrite: access = GL_READ_WRITE; break;
		}

		return glMapNamedBuffer(mBuffer, access);
	}

	void OpenGLBuffer::Resize(size_t size) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		AR_ASSERT(mFlags & BufferFlag::Mutable, "Buffer with immutable flag cannot be resized\n");

		if (mSize == size) return;
		mSize = size;

		glNamedBufferData(mBuffer, mSize, nullptr, mFlags & BufferFlag::Static ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	}

	void OpenGLBuffer::Unmap() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glUnmapNamedBuffer(mBuffer);
	}

	void OpenGLBuffer::SetData(size_t offset, size_t size, const void *data) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		AR_ASSERT(offset + size <= mSize, "Offset + size cannot be larger than buffer size {} + {} > {}\n", offset, size, mSize);
		
		glNamedBufferSubData(mBuffer, offset, size, data);
	}

	void OpenGLBuffer::Destroy() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glDeleteBuffers(1, &mBuffer);
		mBuffer = 0;
	}

	bool OpenGLBuffer::IsValid() const {
		AR_PROFILE_FUNCTION_GPU_CPU();
		return glIsBuffer(mBuffer);
	}

}