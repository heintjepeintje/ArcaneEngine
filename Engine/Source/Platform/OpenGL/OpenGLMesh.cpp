#include "OpenGLMesh.hpp"

namespace Arcane {

	uint32_t GetInputElementTypeCount(const InputElement &element) {
		uint32_t count = 0;
		switch (element.Type) {
			case InputElementType::Vector2: count = 2; break;
			case InputElementType::Vector3: count = 3; break;
			case InputElementType::Vector4: count = 4; break;
			case InputElementType::Vector2i: count = 2; break;
			case InputElementType::Vector3i: count = 3; break;
			case InputElementType::Vector4i: count = 4; break;
			case InputElementType::Vector2u: count = 2; break;
			case InputElementType::Vector3u: count = 3; break;
			case InputElementType::Vector4u: count = 4; break;
			case InputElementType::Float32: count = 1; break;
			case InputElementType::Int32: count = 1; break;
			case InputElementType::UInt32: count = 1; break;
			default: return GL_NONE;
		}
		return count * element.Count;
	}

	uint32_t GetInputElementSize(const InputElement &element) {
		uint32_t size = 0;
		switch (element.Type) {
			case InputElementType::Vector2: size = 2 * sizeof(float); break;
			case InputElementType::Vector3: size = 3 * sizeof(float); break;
			case InputElementType::Vector4: size = 4 * sizeof(float); break;
			case InputElementType::Vector2i: size = 2 * sizeof(int32_t); break;
			case InputElementType::Vector3i: size = 3 * sizeof(int32_t); break;
			case InputElementType::Vector4i: size = 4 * sizeof(int32_t); break;
			case InputElementType::Vector2u: size = 2 * sizeof(uint32_t); break;
			case InputElementType::Vector3u: size = 3 * sizeof(uint32_t); break;
			case InputElementType::Vector4u: size = 4 * sizeof(uint32_t); break;
			case InputElementType::Float32: size = 1 * sizeof(float); break;
			case InputElementType::Int32: size = 1 * sizeof(int32_t); break;
			case InputElementType::UInt32: size = 1 * sizeof(uint32_t); break;
			default: size = 0; break;
		}
		return size * element.Count;
	}

	GLenum GetInputElementOpenGLType(const InputElement &element) {
		switch (element.Type) {
			case InputElementType::Vector2: return GL_FLOAT;
			case InputElementType::Vector3: return GL_FLOAT;
			case InputElementType::Vector4: return GL_FLOAT;
			case InputElementType::Vector2i: return GL_INT;
			case InputElementType::Vector3i: return GL_INT;
			case InputElementType::Vector4i: return GL_INT;
			case InputElementType::Vector2u: return GL_UNSIGNED_INT;
			case InputElementType::Vector3u: return GL_UNSIGNED_INT;
			case InputElementType::Vector4u: return GL_UNSIGNED_INT;
			case InputElementType::Float32: return GL_FLOAT;
			case InputElementType::Int32: return GL_INT;
			case InputElementType::UInt32: return GL_UNSIGNED_INT;
			default: return GL_NONE;
		}
	}

	OpenGLMesh::OpenGLMesh(const std::shared_ptr<OpenGLGraphicsContext> &context) : mContext(context) {
		glCreateVertexArrays(1, &mVertexArray);
	}

	OpenGLMesh::~OpenGLMesh() {
		glDeleteVertexArrays(1, &mVertexArray);
	}

	void OpenGLMesh::SetVertexBuffer(uint32_t index, const InputLayout &layout, const std::shared_ptr<NativeBuffer> &vertexBuffer) {
		AR_ASSERT(vertexBuffer->GetType() == BufferType::Vertex, "Buffer is not a vertex buffer!");
		std::shared_ptr<OpenGLBuffer> buffer = std::static_pointer_cast<OpenGLBuffer>(vertexBuffer);

		mVertexBuffers.emplace(mVertexBuffers.begin() + index, vertexBuffer);

		glVertexArrayVertexBuffer(mVertexArray, index, buffer->GetOpenGLID(), 0, layout.GetTotalSize());

		size_t offset = 0;
		for (size_t i = 0; i < layout.GetElements().size(); i++) {
			glEnableVertexArrayAttrib(mVertexArray, index + i);
			glVertexArrayAttribFormat(mVertexArray, index + i, GetInputElementTypeCount(layout.GetElements()[i]), GetInputElementOpenGLType(layout.GetElements()[i]), GL_FALSE, offset);
			std::printf("[%u]: %u, %u\n", index + i, GetInputElementTypeCount(layout.GetElements()[i]), offset);
			glVertexArrayAttribBinding(mVertexArray, index + i, index);

			offset += GetInputElementSize(layout.GetElements()[i]);
		}

		mLayout.Append(layout);
	}

	void OpenGLMesh::SetIndexBuffer(const std::shared_ptr<NativeBuffer> &indexBuffer) {
		AR_ASSERT(indexBuffer->GetType() == BufferType::Index, "Buffer is not an index buffer!");
		std::shared_ptr<OpenGLBuffer> buffer = std::static_pointer_cast<OpenGLBuffer>(indexBuffer);

		mIndexBuffer = indexBuffer;

		glVertexArrayElementBuffer(mVertexArray, buffer->GetOpenGLID());
	}

	std::shared_ptr<NativeMesh> NativeMesh::Create(const std::shared_ptr<NativeGraphicsContext> &context) {
		return std::make_shared<OpenGLMesh>(
			std::dynamic_pointer_cast<OpenGLGraphicsContext>(context)
		);
	}

}