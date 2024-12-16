#include "OpenGLBuffer.hpp"

namespace Arcane {

	GLenum ToOpenGLBufferType(BufferType type) {
		switch (type) {
			case BufferType::Vertex: return GL_ARRAY_BUFFER;
			case BufferType::Index: return GL_ELEMENT_ARRAY_BUFFER;
			case BufferType::Uniform: return GL_UNIFORM_BUFFER;
			default: return GL_NONE;
		}
	}

	OpenGLBuffer::OpenGLBuffer(BufferType type, size_t size) : mType(type), mSize(size) {
		GLenum openglBufferType = ToOpenGLBufferType(type);

		glCreateBuffers(1, &mBuffer);
		glNamedBufferData(mBuffer, size, nullptr, GL_STATIC_DRAW);
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

	std::shared_ptr<NativeBuffer> NativeBuffer::Create(const std::shared_ptr<NativeGraphicsContext> &context, BufferType type, size_t size) {
		return std::make_shared<OpenGLBuffer>(type, size);
	}

}