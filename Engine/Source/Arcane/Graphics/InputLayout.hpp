#pragma once

#include <Arcane/Core.hpp>
#include <initializer_list>
#include <vector>

namespace Arcane {

	enum class InputAttribute {
		None = 0,
		Position, 
		Normal, 
		UV, 
		Color,
		Tangent,
		Bitangent
	};
	
	enum class InputElementType {
		None = 0,
		Vector2f32, Vector3f32, Vector4f32,
		Vector2i8, Vector3i8, Vector4i8,
		Vector2i16, Vector3i16, Vector4i16,
		Vector2i32, Vector3i32, Vector4i32,
		Vector2u8, Vector3u8, Vector4u8,
		Vector2u16, Vector3u16, Vector4u16,
		Vector2u32, Vector3u32, Vector4u32,
		Float32, Int32, UInt32
	};

	struct InputElement {
		InputAttribute attribute;
		uint32_t Count;
		InputElementType Type;
		bool Normalize;
	};

	class InputLayout {
	public:
		InputLayout(const std::initializer_list<InputElement> &elements);
		InputLayout() { }
		~InputLayout() { }

		inline std::vector<InputElement> GetElements() const { return mElements; }
		inline size_t GetTotalSize() const { return mTotalSize; }

		void Append(const InputLayout &other);

		void operator+=(const InputLayout &other) { Append(other); }
		InputLayout operator+(const InputLayout &other) { InputLayout result = *this; result.Append(other); return result; }

	private:
		size_t mTotalSize;
		std::vector<InputElement> mElements;
	};

}
