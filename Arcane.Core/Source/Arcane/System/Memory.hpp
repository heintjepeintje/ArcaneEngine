#pragma once

#include <Arcane/Core/Core.hpp>
#include "./System.hpp"

#include <utility>

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
	constexpr T&& Forward(typename RemoveReference<T>::Type& value);

	template<typename T>
	constexpr T&& Forward(typename RemoveReference<T>::Type&& value);

	template<typename T, typename... Args>
	T* CreateAt(T* pointer, Args&&... args);

	template<typename T>
	constexpr void DestroyAt(T* pointer);

	class IMemorySystem : public ISystem {
	public:
		virtual ~IMemorySystem() = default;

		virtual B8 Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void *MapMemory(U64 pageCount, PageAccessFlags accessFlags, PageAllocationFlags allocationFlags) = 0;
		virtual void UnmapMemory(void *pointer, U64 size) = 0;
		virtual U64 GetPageSize() const = 0;
	};

	class IAllocator {
	public:
		virtual ~IAllocator() = default;

		virtual void *Allocate(U32 size) = 0;
		virtual void Deallocate(void *pointer) = 0;

		template<typename T>
		inline T* Allocate(U32 count = 1) { return reinterpret_cast<T*>(Allocate(count * sizeof(T))); }
	};

	class MemoryHeap : public IAllocator {
	public:
		struct Entry {
			U32 size;
			U32 isOccupied;
			Entry *next;
		};

		struct Info {
			U64 totalSize;
			U64 allocatedBytes;
			U64 overheadBytes;
			U32 allocationCount;
			U32 entryCount;
		};
	public:
		MemoryHeap(IMemorySystem& memorySystem, U32 pageCount);
		~MemoryHeap();

		void *Allocate(U32 size) override;
		void Deallocate(void *pointer) override;

		void Defragment();

		Info GetInfo() const;

	private:
		IMemorySystem* mMemorySystem;
		void *mAddress;
		U64 mPageCount;
		Entry *mEntries;
	};

	template<typename T>
	class UniquePtr {
	public:
		template<typename... Args>
		static UniquePtr<T> Create(IAllocator& allocator, Args&&... args);

		UniquePtr();
		UniquePtr(const UniquePtr<T>& copy) = delete;
		UniquePtr<T>& operator=(const UniquePtr<T>& copy) = delete;
		UniquePtr(UniquePtr<T>&& move);
		UniquePtr<T>& operator=(UniquePtr<T>&& move);
		~UniquePtr();

		void Drop();

		inline T* GetPointer() const { return mPointer; }
		
		inline T& operator*() const { return *mPointer; }
		inline T* operator->() const { return mPointer; }

	private:
		UniquePtr(T* pointer) : mPointer(pointer) { }

		T* mPointer;
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
		static SharedPtr Create(IAllocator& dataAllocator, IAllocator& objectAllocator, Args&&... args);

		static SharedPtr CreateFrom(IAllocator& dataAllocator, UniquePtr<T>& move);

		SharedPtr();
		SharedPtr(const SharedPtr<T>& copy);
		SharedPtr<T>& operator=(const SharedPtr<T>& copy);
		SharedPtr(SharedPtr<T>&& move);
		SharedPtr<T>& operator=(SharedPtr<T>&& move);
		~SharedPtr();

		void Drop();

		inline T* GetPointer() const { return mPointerData->pointer; }
		inline U64 GetReferenceCount() const { return mPointerData->count; }

		inline T& operator*() const { return *GetPointer(); }
		inline T* operator->() const { return GetPointer(); }

	private:
		struct Data {
			U64 count;
			T* pointer;
		};

		SharedPtr(Data* pointerData) : mPointerData(pointerData) { }

		Data *mPointerData;
	};

	template<typename T>
	constexpr T&& Forward(typename RemoveReference<T>::Type& value) {
		return static_cast<T&&>(value);
	}

	template<typename T>
	constexpr T&& Forward(typename RemoveReference<T>::Type&& value) {
		return static_cast<T&&>(value);
	}

	template<typename T, typename... Args>
	T* CreateAt(T* pointer, Args&&... args) {
		return new (reinterpret_cast<void*>(pointer)) T(Forward<Args>(args)...);
	}

	template<typename T>
	constexpr void DestroyAt(T* pointer) {
		if (pointer == nullptr) return;
		pointer->~T();
	}

	template<typename T>
	template<typename... Args>
	UniquePtr<T> UniquePtr<T>::Create(IAllocator& allocator, Args&&... args) {
		T* pointer = allocator.Allocate<T>();
		CreateAt(pointer, Forward<Args>(args)...);
	
		return UniquePtr<T>(pointer);
	}

	template<typename T>
	UniquePtr<T>::UniquePtr() : mPointer(nullptr) { }

	template<typename T>
	UniquePtr<T>::UniquePtr(UniquePtr<T>&& move) : mPointer(move.mPointer) {
		move.mPointer = nullptr;
	}

	template<typename T>
	UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& move) {
		mPointer = move.mPointer;
		move.mPointer = nullptr;
		return *this;
	}

	template<typename T>
	UniquePtr<T>::~UniquePtr() {
		Drop();
	}

	template<typename T>
	void UniquePtr<T>::Drop() {
		DestroyAt(mPointer);
		mPointer = nullptr;
	}

	template<typename T>
	template<typename... Args>
	SharedPtr<T> SharedPtr<T>::Create(IAllocator& dataAllocator, IAllocator& objectAllocator, Args&&... args) {
		Data* pointerData;

		if (&dataAllocator == &objectAllocator) {
			pointerData = dataAllocator.Allocate(sizeof(Data) + sizeof(T));	
			pointerData->pointer = pointerData + 1;
		} else {
			pointerData = dataAllocator.Allocate<Data>();
			pointerData->pointer = objectAllocator.Allocate<T>();
		}

		CreateAt(pointerData->pointer, Forward<Args>(args)...);
		pointerData->count = 1;

		return SharedPtr<T>(pointerData);
	}

	template<typename T>
	SharedPtr<T> SharedPtr<T>::CreateFrom(IAllocator& dataAllocator, UniquePtr<T>& pointer) {
		Data* pointerData = dataAllocator.Allocate<Data>();
		pointerData->pointer = pointer;
		pointerData->Count = 1;

		pointer->mPointer = nullptr;

		return SharedPtr<T>(pointerData);
	}

	template<typename T>
	SharedPtr<T>::SharedPtr() : mPointerData(nullptr) { }

	template<typename T>
	SharedPtr<T>::SharedPtr(const SharedPtr<T>& copy) : mPointerData(copy.mPointerData) {
		if (mPointerData == nullptr) return;
		mPointerData->count += 1;
	}

	template<typename T>
	SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& copy) {
		Drop();

		mPointerData = copy->mPointerData;
		if (mPointerData == nullptr) return *this;

		mPointerData->count += 1;
		return *this;
	}

	template<typename T>
	SharedPtr<T>::SharedPtr(SharedPtr<T>&& move) : mPointerData(move.mPointerData) {
		if (mPointerData == nullptr) return;
		mPointerData->count += 1;
		move.mPointerData = nullptr;
	}

	template<typename T>
	SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& move) {
		Drop();

		mPointerData = move.mPointerData;
		if (mPointerData == nullptr) return *this;

		mPointerData->count += 1;
		move.mPointerData = nullptr;

		return *this;
	}

	template<typename T>
	SharedPtr<T>::~SharedPtr() {
		Drop();
	}

	template<typename T>
	void SharedPtr<T>::Drop() {
		if (mPointerData == nullptr) return;

		mPointerData->count -= 1;
		if (mPointerData->count == 0) {
			DestroyAt(mPointerData->pointer);	
		}

		mPointerData = nullptr;
	}

}
