#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {
	
	enum class JumpOrigin {
		Begin, Current, End
	};

	class BufferView {
	public:
		BufferView() : mBuffer(nullptr), mSize(0), mOffset(0) { }
		BufferView(void *buffer, size_t size) : mBuffer(buffer), mSize(size), mOffset(0) { }
		BufferView(void *buffer, size_t start, size_t size) : mBuffer(AR_PTR_ADD(buffer, start)), mSize(size), mOffset(0) { }
		BufferView(void *begin, void *end) : mBuffer(begin), mSize((uintptr_t)end - (uintptr_t)begin), mOffset(0) { }
		~BufferView() { }

		template<typename _Type>
		inline _Type &Next() {
			_Type *value = (_Type*)AR_PTR_ADD(mBuffer, mOffset);
			mOffset += sizeof(_Type);
			return *value;
		}

		void Jump(int64_t offset, JumpOrigin origin = JumpOrigin::Current) {
			switch (origin) {
				case JumpOrigin::Begin: mOffset = offset; break;
				case JumpOrigin::Current: mOffset += offset; break;
				case JumpOrigin::End: mOffset = mSize + offset; break;
			}
		}

		inline void *GetData() const { return mBuffer; }
		inline void *GetBegin() const { return mBuffer; }
		inline void *GetEnd() const { return AR_PTR_ADD(mBuffer, mSize); }
		inline size_t GetSize() const { return mSize; }
		inline size_t GetOffset() const { return mOffset; }

	private:
		void *mBuffer;
		size_t mSize;
		size_t mOffset;
	};

}