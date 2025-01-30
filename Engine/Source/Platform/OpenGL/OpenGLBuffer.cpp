#include "OpenGLBuffer.hpp"

namespace Arcane {

	OpenGLBuffer::OpenGLBuffer(size_t size) : mSize(size) {
		glCreateBuffers(1, &mBuffer);
		glNamedBufferStorage(mBuffer, mSize, nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);
	}

	OpenGLBuffer::~OpenGLBuffer() {
		glDeleteBuffers(1, &mBuffer);
	}

	void *OpenGLBuffer::Map(MapMode mode) {
		GLenum access = 0;
		switch (mode) {
			case MapMode::Read: access = GL_READ_ONLY; break;
			case MapMode::Write: access = GL_WRITE_ONLY; break;
			case MapMode::ReadWrite: access = GL_READ_WRITE; break;
		}

		return glMapNamedBuffer(mBuffer, access);
	}

	void OpenGLBuffer::Unmap() {
		glUnmapNamedBuffer(mBuffer);
	}

	void OpenGLBuffer::SetData(size_t offset, size_t size, const void *data) {
		glNamedBufferSubData(mBuffer, offset, size, data);
	}

}