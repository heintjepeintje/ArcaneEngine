#include "InputLayout.hpp"

namespace Arcane {

	uint32_t GetInputElementTypeCount(const InputElement &element) {
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

	uint32_t GetInputElementSize(const InputElement &element) {
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

	InputLayout::InputLayout(const std::initializer_list<InputElement> &element) : mElements(element), mTotalSize(0) {
		for (const InputElement &e : mElements) {
			mTotalSize += GetInputElementSize(e);
		}
	}

	void InputLayout::Append(const InputLayout &other) {
		for (const InputElement &e : other.mElements) {
			mElements.push_back(e);
			mTotalSize += GetInputElementSize(e);
		}
	}

}