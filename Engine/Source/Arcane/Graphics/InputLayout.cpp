#include "InputLayout.hpp"

namespace Arcane {

	size_t GetElementSize(InputElement element) {
		size_t typeSize = 0;
		switch (element.Type) {
			case InputElementType::Vector2: typeSize = 2 * sizeof(float); break;
			case InputElementType::Vector3: typeSize = 3 * sizeof(float); break;
			case InputElementType::Vector4: typeSize = 4 * sizeof(float); break;
			case InputElementType::Vector2i: typeSize = 2 * sizeof(int32_t); break;
			case InputElementType::Vector3i: typeSize = 3 * sizeof(int32_t); break;
			case InputElementType::Vector4i: typeSize = 4 * sizeof(int32_t); break;
			case InputElementType::Vector2u: typeSize = 2 * sizeof(uint32_t); break;
			case InputElementType::Vector3u: typeSize = 3 * sizeof(uint32_t); break;
			case InputElementType::Vector4u: typeSize = 4 * sizeof(uint32_t); break;
			case InputElementType::Float32: typeSize = sizeof(float); break;
			case InputElementType::Int32: typeSize = sizeof(int32_t); break;
			case InputElementType::UInt32: typeSize = sizeof(uint32_t); break;
		}
		return typeSize * element.Count;
	}

	InputLayout::InputLayout(const std::initializer_list<InputElement> &element) : mElements(element), mTotalSize(0) {
		for (InputElement &e : mElements) {
			mTotalSize += GetElementSize(e);
		}
	}

}