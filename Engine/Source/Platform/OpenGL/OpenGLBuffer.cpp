#include "OpenGLBuffer.hpp"

namespace Arcane {

	OpenGLBuffer::OpenGLBuffer(size_t size, uint32_t flags) : mSize(size) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glCreateBuffers(1, &mBuffer);
		if (flags & BufferFlag_Mutable) {
			glNamedBufferData(mBuffer, mSize, nullptr, GL_DYNAMIC_DRAW);
		} else {
			glNamedBufferStorage(mBuffer, mSize, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
		}
	}

	OpenGLBuffer::~OpenGLBuffer() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glDeleteBuffers(1, &mBuffer);
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
		if (mSize == size) return;
		mSize = size;
		glNamedBufferData(mBuffer, mSize, nullptr, GL_DYNAMIC_DRAW);
	}

	void OpenGLBuffer::Unmap() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glUnmapNamedBuffer(mBuffer);
	}

	void OpenGLBuffer::SetData(size_t offset, size_t size, const void *data) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glNamedBufferSubData(mBuffer, offset, size, data);
	}

}