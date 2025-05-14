#include "OpenGLMesh.hpp"

namespace Arcane {

	static uint32_t GetInputElementTypeCount(const InputElement &element) {
		uint32_t count = 0;
		switch (element.Type) {
			case InputElementType::Vector2f32: count = 2; break;
			case InputElementType::Vector3f32: count = 3; break;
			case InputElementType::Vector4f32: count = 4; break;
			case InputElementType::Vector2i8: count = 2; break;
			case InputElementType::Vector3i8: count = 3; break;
			case InputElementType::Vector4i8: count = 4; break;
			case InputElementType::Vector2i16: count = 2; break;
			case InputElementType::Vector3i16: count = 3; break;
			case InputElementType::Vector4i16: count = 4; break;
			case InputElementType::Vector2i32: count = 2; break;
			case InputElementType::Vector3i32: count = 3; break;
			case InputElementType::Vector4i32: count = 4; break;
			case InputElementType::Vector2u8: count = 2; break;
			case InputElementType::Vector3u8: count = 3; break;
			case InputElementType::Vector4u8: count = 4; break;
			case InputElementType::Vector2u16: count = 2; break;
			case InputElementType::Vector3u16: count = 3; break;
			case InputElementType::Vector4u16: count = 4; break;
			case InputElementType::Vector2u32: count = 2; break;
			case InputElementType::Vector3u32: count = 3; break;
			case InputElementType::Vector4u32: count = 4; break;
			case InputElementType::Float32: count = 1; break;
			case InputElementType::Int32: count = 1; break;
			case InputElementType::UInt32: count = 1; break;
		}
		return count * element.Count;
	}

	static uint32_t GetInputElementSize(const InputElement &element) {
		uint32_t size = 0;
		switch (element.Type) {
			case InputElementType::Vector2f32: size = 2 * sizeof(float); break;
			case InputElementType::Vector3f32: size = 3 * sizeof(float); break;
			case InputElementType::Vector4f32: size = 4 * sizeof(float); break;
			case InputElementType::Vector2i8: size = 2 * sizeof(int8_t); break;
			case InputElementType::Vector3i8: size = 3 * sizeof(int8_t); break;
			case InputElementType::Vector4i8: size = 4 * sizeof(int8_t); break;
			case InputElementType::Vector2i16: size = 2 * sizeof(int16_t); break;
			case InputElementType::Vector3i16: size = 3 * sizeof(int16_t); break;
			case InputElementType::Vector4i16: size = 4 * sizeof(int16_t); break;
			case InputElementType::Vector2i32: size = 2 * sizeof(int32_t); break;
			case InputElementType::Vector3i32: size = 3 * sizeof(int32_t); break;
			case InputElementType::Vector4i32: size = 4 * sizeof(int32_t); break;
			case InputElementType::Vector2u8: size = 2 * sizeof(uint8_t); break;
			case InputElementType::Vector3u8: size = 3 * sizeof(uint8_t); break;
			case InputElementType::Vector4u8: size = 4 * sizeof(uint8_t); break;
			case InputElementType::Vector2u16: size = 2 * sizeof(uint16_t); break;
			case InputElementType::Vector3u16: size = 3 * sizeof(uint16_t); break;
			case InputElementType::Vector4u16: size = 4 * sizeof(uint16_t); break;
			case InputElementType::Vector2u32: size = 2 * sizeof(uint32_t); break;
			case InputElementType::Vector3u32: size = 3 * sizeof(uint32_t); break;
			case InputElementType::Vector4u32: size = 4 * sizeof(uint32_t); break;
			case InputElementType::Float32: size = sizeof(float); break;
			case InputElementType::Int32: size = sizeof(int32_t); break;
			case InputElementType::UInt32: size = sizeof(uint32_t); break;
			default: size = 0; break;
		}
		return size * element.Count;
	}

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
		glDeleteVertexArrays(1, &mVertexArray);
	}

	void OpenGLMesh::SetVertexBuffer(uint32_t index, const InputLayout &layout, const Ref<NativeBuffer> &vertexBuffer) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		Ref<OpenGLBuffer> buffer = CastRef<OpenGLBuffer>(vertexBuffer);

		mVertexBuffers.emplace(mVertexBuffers.begin() + index, vertexBuffer);

		glVertexArrayVertexBuffer(mVertexArray, index, buffer->GetOpenGLID(), 0, layout.GetTotalSize());

		size_t offset = 0;
		for (size_t i = 0; i < layout.GetElements().size(); i++) {
			glEnableVertexArrayAttrib(mVertexArray, index + i);
			glVertexArrayAttribFormat(
				mVertexArray, 
				index + i, 
				GetInputElementTypeCount(layout.GetElements()[i]), 
				GetInputElementOpenGLType(layout.GetElements()[i]), 
				layout.GetElements()[i].Normalize ? GL_TRUE : GL_FALSE, 
				offset
			);
			glVertexArrayAttribBinding(mVertexArray, index + i, index);

			offset += GetInputElementSize(layout.GetElements()[i]);
		}

		mLayout.Append(layout);
	}

	void OpenGLMesh::SetIndexBuffer(const Ref<NativeBuffer> &indexBuffer) {
		AR_PROFILE_FUNCTION_GPU_CPU();
		Ref<OpenGLBuffer> buffer = CastRef<OpenGLBuffer>(indexBuffer);

		mIndexBuffer = indexBuffer;

		glVertexArrayElementBuffer(mVertexArray, buffer->GetOpenGLID());
	}

	Ref<NativeBuffer> OpenGLMesh::GetVertexBuffer(uint32_t index) {
		return mVertexBuffers[index];
	}

	Ref<NativeBuffer> OpenGLMesh::GetIndexBuffer() {
		return mIndexBuffer;
	}

	

}