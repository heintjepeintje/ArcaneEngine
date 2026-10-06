#pragma once

#include <Arcane/Core/Core.hpp>

namespace Arcane {

	template<typename T = U8>
	class Slice {
	public:
		Slice() = default;
		Slice(T* pointer);
		Slice(T* pointer, U64 length);
		Slice(T* begin, T* end);

		template<typename U>
		inline Slice<U> reinterpret_as() const;

		inline Slice<T> slice(U64 offset, U64 length);

		inline T* pointer() const { return m_pointer; }
		inline U64 length() const { return m_length; }

	private:
		T* m_pointer;
		U64 m_length;
	};

	template<typename T>
	Slice<T>::Slice(T* pointer) : m_pointer(pointer), m_length(1) { }

	template<typename T>
	Slice<T>::Slice(T* pointer, U64 length) : m_pointer(pointer), m_length(length) { }

	template<typename T>
	Slice<T>::Slice(T* begin, T* end) : m_pointer(begin), m_length(end - begin) { }

	template<typename T>
	template<typename U>
	Slice<U> Slice<T>::reinterpret_as() const {
		static_assert(sizeof(U) % sizeof(T) == 0);
		return Slice<U>{ reinterpret_cast<U*>(m_pointer), (m_length * sizeof(T)) / sizeof(U); }
	}

	template<typename T>
	Slice<T> Slice<T>::slice(U64 offset, U64 length) {
		if (offset + length > m_length) return Slice{};
		return Slice<T>{ m_pointer + offset, length }
	}

}