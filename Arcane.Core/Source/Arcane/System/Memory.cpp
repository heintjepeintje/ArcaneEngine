#include "./Memory.hpp"

#include <cstdio>

namespace Arcane {

	MemoryHeap::MemoryHeap(IMemorySystem& memorySystem, U32 pageCount)
		: mMemorySystem(&memorySystem), mPageCount(pageCount) {
		if (mPageCount == 0) return;
		
		mAddress = mMemorySystem->MapMemory(
				mPageCount,
				PAGE_ACCESS_READ | PAGE_ACCESS_WRITE,
				PAGE_ALLOCATION_PRIVATE);

		mEntries = reinterpret_cast<Entry*>(mAddress);
		mEntries->isOccupied = false;
		mEntries->size = mPageCount * mMemorySystem->GetPageSize() - sizeof(Entry);
		mEntries->next = nullptr;
	}

	MemoryHeap::~MemoryHeap() {
		mMemorySystem->UnmapMemory(mAddress, mPageCount * mMemorySystem->GetPageSize());
	}

	void *MemoryHeap::Allocate(U32 size) {
		Entry *currentEntry = mEntries;

		while (currentEntry != nullptr) {
			if (!currentEntry->isOccupied && currentEntry->size >= size) break;

			currentEntry = currentEntry->next;
		}

		if (currentEntry == nullptr) {
			std::printf("Could not find a fitting heap entry.\n");
			return nullptr;
		}

		std::printf("Found entry of %zu bytes at %p\n", currentEntry->size, currentEntry);

		const U32 remainingBytes = currentEntry->size - size;
		U32 actualSize = (size % 16) == 0 ? size : size + (16 - (size % 16));
		void *address = (currentEntry + 1);

		Entry *nextAddress = nullptr;

		if (remainingBytes <= 16) {
			actualSize += remainingBytes;
			nextAddress = currentEntry->next;
		} else {
			nextAddress = reinterpret_cast<Entry*>(reinterpret_cast<U8*>(mAddress) + actualSize);
			nextAddress->next = currentEntry->next;
			nextAddress->size = remainingBytes - 16;

			std::printf("Creating new block at %p of size %zu.\n", nextAddress, nextAddress->size);

			currentEntry->next = nextAddress;
		}

		currentEntry->isOccupied = true;
		currentEntry->size = actualSize;

		return address;
	}

	void MemoryHeap::Deallocate(void *pointer) {
		if (pointer == nullptr) return;		
		// TODO: Validate the address to verify it is in this memory heap.
		
		Entry *entry = reinterpret_cast<MemoryHeap::Entry*>(pointer) - 1;
		if (!entry->isOccupied) return;

		entry->isOccupied = false;
	}

	void MemoryHeap::Defragment() {
		Entry *currentEntry = mEntries;

		while (currentEntry->next != nullptr) {
			Entry *NextEntry = currentEntry->next;
			if (!currentEntry->isOccupied && !NextEntry->isOccupied) {
				currentEntry->size += NextEntry->size + sizeof(Entry);
				currentEntry->next = NextEntry->next;
				continue;
			}

			currentEntry = NextEntry;
		}
	}

	MemoryHeap::Info MemoryHeap::GetInfo() const {
		Entry* currentEntry = mEntries;

		Info info = {};
		info.totalSize = mPageCount * mMemorySystem->GetPageSize();
		
		while (currentEntry != nullptr) {
			if (currentEntry->isOccupied) {
				info.allocatedBytes += currentEntry->size;
				info.allocationCount++;
			}
	
			info.entryCount++;
			info.overheadBytes += sizeof(Entry);

			currentEntry = currentEntry->next;
		}

		return info;
	}

}
