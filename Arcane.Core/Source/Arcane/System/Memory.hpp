#pragma once

#include <Arcane/Core/Core.hpp>
#include "./System.hpp"

namespace Arcane {

	enum PageAccessFlagBits : U32 {
		PAGE_ACCESS_READ = (1 << 0),
		PAGE_ACCESS_WRITE = (1 << 1),
		PAGE_ACCESS_EXECUTE = (1 << 2)
	};

	using PageAccessFlags = U32;

	enum PageAllocationFlagBits : U32 {
		PAGE_ALLOCATION_PRIVATE = (1 << 0)
	};

	using PageAllocationFlags = U32;

	template<typename T>
	constexpr T&& forward(typename RemoveReference<T>::Type& value);

	template<typename T>
	constexpr T&& forward(typename RemoveReference<T>::Type&& value);

	template<typename T, typename... Args>
	T* create_at(T* pointer, Args&&... args);

	template<typename T>
	constexpr void destroy_at(T* pointer);

	void *copy(void *source, void *destination, U64 length);
	void *fill(void *pointer, U64 length, U8 byte);

	class IMemorySystem : public ISystem {
	public:
		virtual ~IMemorySystem() = default;

		virtual B8 initialize() = 0;
		virtual void shutdown() = 0;

		virtual void *map_memory(U64 page_count, PageAccessFlags access_flags, PageAllocationFlags allocation_flags) = 0;
		virtual void unmap_memory(void *pointer, U64 size) = 0;
		virtual U64 page_size() const = 0;
	};

	class IAllocator {
	public:
		virtual ~IAllocator() = default;

		virtual void *allocate(U32 size) = 0;
		virtual void *reallocate(void *pointer, U32 size) = 0;
		virtual void deallocate(void *pointer) = 0;

		template<typename T>
		inline T* allocate(U32 count = 1) { return reinterpret_cast<T*>(allocate(count * sizeof(T))); }
	};

	enum MemoryHeapFlagBits {
		MEMORY_HEAP_DEFRAGMENT_ON_DEALLOCATE = (1 << 0),
		MEMORY_HEAP_ZERO_ON_ALLOCATE = (1 << 1),
		MEMORY_HEAP_ZERO_ON_DEALLOCATE = (1 << 2)
	};

	using MemoryHeapFlags = U32;

	class MemoryHeap : public IAllocator {
	public:
		struct Entry {
			U32 size;
			U32 is_occupied;
			Entry *next;
		};
	public:
		MemoryHeap() { }
		MemoryHeap(IMemorySystem& memorySystem, U32 size, MemoryHeapFlags flags);
		~MemoryHeap();

		void *allocate(U32 size) override;
		void *reallocate(void *pointer, U32 size) override;
		void deallocate(void *pointer) override;

		void Defragment();

		inline U64 size() const { return m_page_count * m_memory_system->page_size(); }

	private:
		static U32 align_size(U32 size);
		static void *calculate_entry_address(Entry* entry);
		static Entry *calculate_address_entry(void *address);
		static B8 can_expand_entry(Entry* entry, U32 size);
		
		void concatenate_consecutive_entries(Entry* entry);
		B8 split_entry(Entry* entry, U32 minimumSize);
		
		Entry* find_unoccupied_entry(U32 minimumSize);

		IMemorySystem* m_memory_system = nullptr;
		U64 m_page_count = 0;
		Entry *m_entries = nullptr;
		MemoryHeapFlags m_flags = 0;
	};

	template<typename T>
	class UniquePtr {
	public:
		template<typename... Args>
		static UniquePtr<T> create(IAllocator& allocator, Args&&... args);

		UniquePtr();
		UniquePtr(const UniquePtr<T>& copy) = delete;
		UniquePtr<T>& operator=(const UniquePtr<T>& copy) = delete;
		UniquePtr(UniquePtr<T>&& move);
		UniquePtr<T>& operator=(UniquePtr<T>&& move);
		~UniquePtr();

		void drop();

		inline T* pointer() const { return m_pointer; }
		
		inline T& operator*() const { return *pointer(); }
		inline T* operator->() const { return pointer(); }

	private:
		UniquePtr(T* pointer) : m_pointer(pointer) { }

		T* m_pointer;
	};

	template<typename T>
	class SharedPtr {
	public:
		friend class UniquePtr<T>;

		// Creates a new shared pointer instance. 
		// If ```dataAllocator``` and ```objectAllocator``` are the same, it will allocate one contiguous block of memory containing both the
		// shared pointer data and the object. 
		// @param dataAllocator The allocator to allocate the shared pointer data.
		// @param objectAllocator The allocator to allocate the object.
		template<typename... Args>
		static SharedPtr create(IAllocator& data_allocator, IAllocator& object_allocator, Args&&... args);

		static SharedPtr create_from(IAllocator& data_allocator, UniquePtr<T>& move);

		SharedPtr();
		SharedPtr(const SharedPtr<T>& copy);
		SharedPtr<T>& operator=(const SharedPtr<T>& copy);
		SharedPtr(SharedPtr<T>&& move);
		SharedPtr<T>& operator=(SharedPtr<T>&& move);
		~SharedPtr();

		void drop();

		inline T* pointer() const { return m_pointer_data->pointer; }
		inline U64 reference_count() const { return m_pointer_data->count; }

		inline T& operator*() const { return *pointer(); }
		inline T* operator->() const { return pointer(); }

	private:
		struct Data {
			U64 count;
			T* pointer;
		};

		SharedPtr(Data* pointer_data) : m_pointer_data(pointer_data) { }

		Data *m_pointer_data;
	};

	/************************
	 **** IMPLEMENTATION ****
	 ************************/

	template<typename T>
	constexpr T&& forward(typename RemoveReference<T>::Type& value) {
		return static_cast<T&&>(value);
	}

	template<typename T>
	constexpr T&& forward(typename RemoveReference<T>::Type&& value) {
		return static_cast<T&&>(value);
	}

	template<typename T, typename... Args>
	T* create_at(T* pointer, Args&&... args) {
		return new (reinterpret_cast<void*>(pointer)) T(forward<Args>(args)...);
	}

	template<typename T>
	constexpr void destroy_at(T* pointer) {
		if (pointer == nullptr) return;
		pointer->~T();
	}

	template<typename T>
	template<typename... Args>
	UniquePtr<T> UniquePtr<T>::create(IAllocator& allocator, Args&&... args) {
		T* pointer = allocator.allocate<T>();
		create_at(pointer, forward<Args>(args)...);
	
		return UniquePtr<T>(pointer);
	}

	template<typename T>
	UniquePtr<T>::UniquePtr() : m_pointer(nullptr) { }

	template<typename T>
	UniquePtr<T>::UniquePtr(UniquePtr<T>&& move) : m_pointer(move.m_pointer) {
		move.m_pointer = nullptr;
	}

	template<typename T>
	UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& move) {
		m_pointer = move.m_pointer;
		move.m_pointer = nullptr;
		return *this;
	}

	template<typename T>
	UniquePtr<T>::~UniquePtr() {
		drop();
	}

	template<typename T>
	void UniquePtr<T>::drop() {
		destroy_at(m_pointer);
		m_pointer = nullptr;
	}

	template<typename T>
	template<typename... Args>
	SharedPtr<T> SharedPtr<T>::create(IAllocator& data_allocator, IAllocator& object_allocator, Args&&... args) {
		Data* pointer_data;

		if (&data_allocator == &object_allocator) {
			pointer_data = data_allocator.allocate(sizeof(Data) + sizeof(T));	
			pointer_data->pointer = pointer_data + 1;
		} else {
			pointer_data = data_allocator.allocate<Data>();
			pointer_data->pointer = object_allocator.allocate<T>();
		}

		create_at(pointer_data->pointer, forward<Args>(args)...);
		pointer_data->count = 1;

		return SharedPtr<T>(pointer_data);
	}

	template<typename T>
	SharedPtr<T> SharedPtr<T>::create_from(IAllocator& data_allocator, UniquePtr<T>& pointer) {
		Data* pointer_data = data_allocator.allocate<Data>();
		pointer_data->pointer = pointer;
		pointer_data->Count = 1;

		pointer->m_pointer = nullptr;

		return SharedPtr<T>(pointer_data);
	}

	template<typename T>
	SharedPtr<T>::SharedPtr() : m_pointer_data(nullptr) { }

	template<typename T>
	SharedPtr<T>::SharedPtr(const SharedPtr<T>& copy) : m_pointer_data(copy.m_pointer_data) {
		if (m_pointer_data == nullptr) return;
		m_pointer_data->count += 1;
	}

	template<typename T>
	SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& copy) {
		drop();

		m_pointer_data = copy->m_pointer_data;
		if (m_pointer_data == nullptr) return *this;

		m_pointer_data->count += 1;
		return *this;
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(SharedPtr<T>&& move) : m_pointer_data(move.m_pointer_data) {
		if (m_pointer_data == nullptr) return;
		m_pointer_data->count += 1;
		move.m_pointer_data = nullptr;
	}

	template<typename T>
	SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& move) {
		drop();

		m_pointer_data = move.m_pointer_data;
		if (m_pointer_data == nullptr) return *this;

		m_pointer_data->count += 1;
		move.m_pointer_data = nullptr;

		return *this;
	}

	template<typename T>
	SharedPtr<T>::~SharedPtr() {
		drop();
	}

	template<typename T>
	void SharedPtr<T>::drop() {
		if (m_pointer_data == nullptr) return;

		m_pointer_data->count -= 1;
		if (m_pointer_data->count == 0) {
			destroy_at(m_pointer_data->pointer);	
		}

		m_pointer_data = nullptr;
	}

}
