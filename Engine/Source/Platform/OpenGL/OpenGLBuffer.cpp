#include "OpenGLBuffer.hpp"

namespace Arcane {

	OpenGLBuffer::OpenGLBuffer(size_t size, uint32_t flags) : mSize(size) {
		glCreateBuffers(1, &mBuffer);
		if (flags & BufferFlag_Mutable) {
			GLenum usage = 0;
			if (flags & BufferFlag_Static) {
				usage = GL_STATIC_DRAW;
			} else {
				usage = GL_DYNAMIC_DRAW;
			}

			glNamedBufferData(mBuffer, mSize, nullptr, usage);
		} else {
			GLenum flags = 0;
			if (flags & ~BufferFlag_Static) flags |= GL_DYNAMIC_STORAGE_BIT;
			if (flags & BufferFlag_MapRead) flags |= GL_MAP_READ_BIT;
			if (flags & BufferFlag_MapWrite) flags |= GL_MAP_WRITE_BIT;
			glNamedBufferStorage(mBuffer, mSize, nullptr, flags);
		}
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