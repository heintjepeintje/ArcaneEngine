#include "BufferData.hpp"

#include <Arcane/System/Memory.hpp>

namespace Arcane {

	BufferRef::BufferRef() : mData(nullptr) {}

	BufferRef::BufferRef(BufferData *data) : mData(data) {
		if (!mData) return;
		mData->RefCount++;
	}

	BufferRef::BufferRef(const BufferRef &other) : mData(other.mData) {
		if (!other.mData) return;
		mData->RefCount++;
	}

	BufferRef &BufferRef::operator=(const BufferRef &other) {
		Drop();

		mData = other.mData;
		if (mData != nullptr) mData->RefCount++;

		return *this;
	}

	BufferRef::~BufferRef() {
		Drop();
	}

	void BufferRef::Drop() {
		if (!mData) return;
		mData->RefCount--;

		if (mData->RefCount != 0) return;
		
		Free(mData->Pointer);
		Free(mData);

		mData = nullptr;
	}

	BufferRef AllocateBuffer(size_t size) {
		BufferData *data = AllocateObject<BufferData>();
		data->Size = size;
		data->RefCount = 0;
		data->Pointer = Allocate(data->Size);

		return BufferRef(data);
	}

}