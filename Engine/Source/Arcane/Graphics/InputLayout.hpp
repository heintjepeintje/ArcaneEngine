#pragma once

#include <Arcane/Core.hpp>
#include <initializer_list>
#include <vector>

namespace Arcane {

	enum class InputAttribute {
		None = 0,
		Position, Normal, UV, Color
	};
	
	enum class InputElementType {
		None = 0,
		Vector2, Vector3, Vector4,
		Vector2i, Vector3i, Vector4i,
		Vector2u, Vector3u, Vector4u,
		Float32, Int32, UInt32
	};

	struct InputElement {
		InputAttribute attribute;
		uint32_t Count;
		InputElementType Type;
	};

	class InputLayout {	
	public:
		InputLayout(const std::initializer_list<InputElement> &elements);
		InputLayout() { }
		~InputLayout() { }

		inline std::vector<InputElement> GetElements() const { return mElements; }
		inline size_t GetTotalSize() const { return mTotalSize; }

		void Append(const InputLayout &other);

	private:
		size_t mTotalSize;
		std::vector<InputElement> mElements;
	};

}
