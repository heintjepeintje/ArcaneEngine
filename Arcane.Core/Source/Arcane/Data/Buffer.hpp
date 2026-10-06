#pragma once

#include <Arcane/System/Memory.hpp>

namespace Arcane {

	template<typename T = U8>
	class Buffer {
	public:
		static Buffer allocate(IAllocator& allocator, U64 capacity);
		static void copy(
			const Buffer<const typename RemoveConst<T>::Type>& source,
			const Buffer<typename RemoveConst<T>::Type>& destination,
			U64 offset, U64 length);

		Buffer();
		Buffer(const Buffer<T>& copy) = delete;
		Buffer<T>& operator=(const Buffer<T>& copy) = delete;
		Buffer(Buffer<T>&& move);
		Buffer<T>& operator=(Buffer<T>&& move);
		~Buffer();
		
		void deallocate();

		inline U64 capacity() const { return m_capacity; }
		inline T* data() const { return m_data; }

	private:
		IAllocator* m_allocator;
		T* m_data = nullptr;
		U64 m_capacity = 0;
	};

	template<typename T>
	Buffer<T> Buffer<T>::allocate(IAllocator& allocator, U64 capacity) {
		Buffer<T> buffer;
		
		buffer.m_allocator = &allocator;
		buffer.m_data = buffer.m_allocator->allocate<T>(capacity);
		buffer.m_capacity = capacity;

		return buffer;
	}

	template<typename T>
	void Buffer<T>::copy(
		const Buffer<const typename RemoveConst<T>::Type>& source,
		const Buffer<typename RemoveConst<T>::Type>& destination,
		U64 offset, U64 length) {
		Arcane::copy(
			source.m_data + offset,
			destination.m_data, length * sizeof(T));
	}

	template<typename T>
	Buffer<T>::Buffer() { }

	template<typename T>
	Buffer<T>::Buffer(Buffer<T>&& move) : m_data(move.m_data), m_capacity(move.m_capacity) {
		move.m_data = nullptr;
		move.m_capacity = 0;
	}

	template<typename T>
	Buffer<T>& Buffer<T>::operator=(Buffer<T>&& move) {
		deallocate();
		
		m_data = move.m_data;
		m_capacity = move.m_capacity;

		move.m_data = nullptr;
		move.m_capacity = 0;
	}

	template<typename T>
	Buffer<T>::~Buffer() {
		deallocate();
	}

	template<typename T>
	void Buffer<T>::deallocate() {
		if (m_data == nullptr) return;
		
		m_allocator->deallocate(m_data);

		m_data = nullptr;
		m_capacity = 0;
	}

}