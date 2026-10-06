#include "./Memory.hpp"

#include <cstdio>

namespace Arcane {

	void *copy(void *source, void *destination, U64 length) {
		return __builtin_memcpy(destination, source, length);
	}

	void *fill(void *pointer, U64 length, U8 byte) {
		return __builtin_memset(pointer, byte, length);
	}

	MemoryHeap::MemoryHeap(IMemorySystem& memorySystem, U32 size, MemoryHeapFlags flags)
		: m_memory_system(&memorySystem), m_flags(flags) {

		const U64 page_size = m_memory_system->page_size();

		m_page_count = (size % page_size == 0) ? size / page_size : (size / page_size) + 1;
		
		m_entries = reinterpret_cast<Entry*>(m_memory_system->map_memory(
				m_page_count,
				PAGE_ACCESS_READ | PAGE_ACCESS_WRITE,
				PAGE_ALLOCATION_PRIVATE));

		if (flags & MEMORY_HEAP_ZERO_ON_ALLOCATE) {
			fill(m_entries, m_page_count * m_memory_system->page_size(), 0);
		}

		m_entries->is_occupied = false;
		m_entries->size = m_page_count * m_memory_system->page_size() - sizeof(Entry);
		m_entries->next = nullptr;
	}

	MemoryHeap::~MemoryHeap() {
		if (m_memory_system == nullptr) return;
		m_memory_system->unmap_memory(m_entries, m_page_count * m_memory_system->page_size());
	}

	void *MemoryHeap::allocate(U32 size) {
		if (m_memory_system == nullptr) return nullptr;

		const U32 aligned_size = align_size(size);

		Entry *found_entry = find_unoccupied_entry(aligned_size);
		if (found_entry == nullptr) return nullptr;

		B8 split_entry_succeeded = split_entry(found_entry, aligned_size);
		if (!split_entry_succeeded) return nullptr;

		void *address = calculate_entry_address(found_entry);

		if (m_flags & MEMORY_HEAP_ZERO_ON_ALLOCATE) {
			fill(address, found_entry->size, 0);
		}

		return address;
	}

	void *MemoryHeap::reallocate(void *pointer, U32 size) {
		if (m_memory_system == nullptr) return nullptr;

		Entry *entry = calculate_address_entry(pointer);
		const U32 aligned_size = align_size(size);

		if (entry->size >= aligned_size) return pointer;
		if (!entry->is_occupied) return nullptr;

		if (can_expand_entry(entry, aligned_size)) {
			concatenate_consecutive_entries(entry);
			
			B8 split_entry_succeeded = split_entry(entry, aligned_size);
			if (!split_entry_succeeded) return nullptr;

			return pointer;
		} else {
			void *address = allocate(aligned_size);
			if (address == nullptr) return nullptr;

			void *destination_address = copy(pointer, address, entry->size);

			if (destination_address != address) return nullptr;
			deallocate(pointer);

			return address;
		}
	}

	void MemoryHeap::deallocate(void *pointer) {
		if (m_memory_system == nullptr) return;
		if (pointer == nullptr) return;
		// TODO: Validate the address to verify it is in this memory heap.
		
		Entry *entry = reinterpret_cast<MemoryHeap::Entry*>(pointer) - 1;
		if (!entry->is_occupied) return;

		entry->is_occupied = false;

		if (m_flags & MEMORY_HEAP_ZERO_ON_DEALLOCATE) {
			fill(calculate_entry_address(entry), entry->size, 0);			
		}

		if (m_flags & MEMORY_HEAP_DEFRAGMENT_ON_DEALLOCATE) {
			if (entry->next != nullptr && !entry->next->is_occupied) {
				concatenate_consecutive_entries(entry);
			}

			if (entry == m_entries) return;

			Entry *current_entry = m_entries;
			while (current_entry->next != nullptr) {
				if (current_entry->next != entry) continue;
				if (current_entry->next->is_occupied) break;

				concatenate_consecutive_entries(current_entry);
			}
		}
	}

	void MemoryHeap::Defragment() {
		if (m_memory_system == nullptr) return;
		Entry *current_entry = m_entries;

		while (current_entry->next != nullptr) {
			if (!current_entry->is_occupied && !current_entry->next->is_occupied) {
				concatenate_consecutive_entries(current_entry);
			} else {
				current_entry = current_entry->next;
			}
		}
	}

	U32 MemoryHeap::align_size(U32 size) {
		return (size % 16) == 0 ? size : size + (16 - (size % 16));
	}

	void *MemoryHeap::calculate_entry_address(Entry* entry) {
		return reinterpret_cast<void*>(entry + 1);
	}

	MemoryHeap::Entry *MemoryHeap::calculate_address_entry(void *address) {
		return reinterpret_cast<Entry*>(address) - 1;
	}

	B8 MemoryHeap::can_expand_entry(Entry* entry, U32 size) {
		return
			entry->next != nullptr &&
			!entry->next->is_occupied &&
			entry->next->size + sizeof(Entry) >= size;
	}

	void MemoryHeap::concatenate_consecutive_entries(Entry* entry) {
		if (m_memory_system == nullptr) return;
		if (entry->next == nullptr) return;

		entry->size += entry->next->size + sizeof(Entry);
		
		Entry* next_entry = entry->next;
		entry->next = entry->next->next;

		if (m_flags & MEMORY_HEAP_ZERO_ON_DEALLOCATE) {
			fill(next_entry, sizeof(Entry), 0);
		}
	}

	B8 MemoryHeap::split_entry(Entry* entry, U32 minimum_size) {
		if (m_memory_system == nullptr) return false;
		if (minimum_size >= entry->size) return false;
		if (minimum_size % 16 != 0) return false;

		const U32 remaining_bytes = entry->size - minimum_size;
		U32 actual_size = minimum_size;

		if (remaining_bytes <= sizeof(Entry)) {
			actual_size += remaining_bytes;
		} else {
			Entry* new_entry = reinterpret_cast<Entry*>(reinterpret_cast<U8*>(entry) + actual_size);
			new_entry->size = remaining_bytes - sizeof(Entry);
			new_entry->next = entry->next;
			new_entry->is_occupied = false;

			entry->next = new_entry;
		}

		entry->size = actual_size;

		return true;
	}

	MemoryHeap::Entry* MemoryHeap::find_unoccupied_entry(U32 size) {
		Entry* current_entry = m_entries;

		while (current_entry != nullptr) {
			if (!current_entry->is_occupied && current_entry->size >= size) {	
				return current_entry;
			}
			current_entry = current_entry->next;
		}

		return nullptr;
	}

}
