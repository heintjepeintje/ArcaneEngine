#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	struct BufferData {
		size_t Size;
		void *Pointer;
		uint32_t RefCount;
	};

	class BufferRef {
	public:
		BufferRef();
		BufferRef(BufferData *data);
		BufferRef(const BufferRef &other);
		BufferRef &operator=(const BufferRef &other);
		~BufferRef();

		inline void *GetPointer() { return mData ? mData->Pointer : nullptr; }
		inline const void *GetPointer() const { return mData ? mData->Pointer : nullptr; }
		inline size_t GetSize() const { return mData ? mData->Size : 0; }
		inline uint32_t GetRefCount() const { return mData ? mData->RefCount : 0; }

		template<typename _Type>
		inline _Type *GetPointerAs() { return static_cast<_Type*>(GetPointer()); }

		template<typename _Type>
		inline const _Type *GetPointerAs() const { return static_cast<const _Type*>(GetPointer()); }

		inline void Drop();

		inline bool IsValid() const { return mData != nullptr; }
		inline operator bool() const { return IsValid(); }

	private:
		BufferData *mData;
	};
	
	BufferRef AllocateBuffer(size_t size);

}