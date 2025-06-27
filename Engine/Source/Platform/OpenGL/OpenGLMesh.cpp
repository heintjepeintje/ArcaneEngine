#include "OpenGLMesh.hpp"

namespace Arcane {

	static GLenum GetInputElementOpenGLType(const InputElement &element) {
		switch (element.Type) {
			case InputElementType::Vector2f32: return GL_FLOAT;
			case InputElementType::Vector3f32: return GL_FLOAT;
			case InputElementType::Vector4f32: return GL_FLOAT;
			case InputElementType::Vector2i8: return GL_BYTE;
			case InputElementType::Vector3i8: return GL_BYTE;
			case InputElementType::Vector4i8: return GL_BYTE;
			case InputElementType::Vector2i16: return GL_SHORT;
			case InputElementType::Vector3i16: return GL_SHORT;
			case InputElementType::Vector4i16: return GL_SHORT;
			case InputElementType::Vector2i32: return GL_INT;
			case InputElementType::Vector3i32: return GL_INT;
			case InputElementType::Vector4i32: return GL_INT;
			case InputElementType::Vector2u8: return GL_UNSIGNED_BYTE;
			case InputElementType::Vector3u8: return GL_UNSIGNED_BYTE;
			case InputElementType::Vector4u8: return GL_UNSIGNED_BYTE;
			case InputElementType::Vector2u16: return GL_UNSIGNED_SHORT;
			case InputElementType::Vector3u16: return GL_UNSIGNED_SHORT;
			case InputElementType::Vector4u16: return GL_UNSIGNED_SHORT;
			case InputElementType::Vector2u32: return GL_UNSIGNED_INT;
			case InputElementType::Vector3u32: return GL_UNSIGNED_INT;
			case InputElementType::Vector4u32: return GL_UNSIGNED_INT;
			case InputElementType::Float32: return GL_FLOAT;
			case InputElementType::Int32: return GL_INT;
			case InputElementType::UInt32: return GL_UNSIGNED_INT;
			default: return GL_NONE;
		}
	}

	OpenGLMesh::OpenGLMesh(const Ref<OpenGLGraphicsContext> &context) : mContext(context) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		glCreateVertexArrays(1, &mVertexArray);
	}

	OpenGLMesh::~OpenGLMesh() {
		AR_PROFILE_FUNCTION_GPU_CPU();
		Destroy();
	}

	void OpenGLMesh::SetVertexBuffer(uint32_t index, const InputLayout &layout, const Ref<NativeBuffer> &vertexBuffer, size_t offset, size_t stride) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		Ref<OpenGLBuffer> buffer = CastRef<OpenGLBuffer>(vertexBuffer);

		mVertexBuffers.emplace(mVertexBuffers.begin() + index, vertexBuffer);

		glVertexArrayVertexBuffer(mVertexArray, index, buffer->GetOpenGLID(), offset, stride);

		size_t relativeOffset = 0;
		for (size_t i = 0; i < layout.GetElements().size(); i++) {
			glEnableVertexArrayAttrib(mVertexArray, index + i);
			glVertexArrayAttribFormat(
				mVertexArray, 
				index + i, 
				GetInputElementTypeCount(layout.GetElements()[i]), 
				GetInputElementOpenGLType(layout.GetElements()[i]), 
				layout.GetElements()[i].Normalize ? GL_TRUE : GL_FALSE, 
				relativeOffset
			);
			glVertexArrayAttribBinding(mVertexArray, index + i, index);

			relativeOffset += GetInputElementSize(layout.GetElements()[i]);
		}

		mLayout.Append(layout);
	}

	void OpenGLMesh::SetIndexBuffer(const Ref<NativeBuffer> &indexBuffer) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		Ref<OpenGLBuffer> buffer = CastRef<OpenGLBuffer>(indexBuffer);

		mIndexBuffer = indexBuffer;

		glVertexArrayElementBuffer(mVertexArray, buffer->GetOpenGLID());
	}

	void OpenGLMesh::Destroy() {
		glDeleteVertexArrays(1, &mVertexArray);
	}

	bool OpenGLMesh::IsValid() const {
		return glIsVertexArray(mVertexArray);
	}

	Ref<NativeBuffer> OpenGLMesh::GetVertexBuffer(uint32_t index) {
		return mVertexBuffers[index];
	}

	Ref<NativeBuffer> OpenGLMesh::GetIndexBuffer() {
		return mIndexBuffer;
	}

	

}