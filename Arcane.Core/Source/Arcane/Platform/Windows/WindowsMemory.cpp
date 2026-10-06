#include "./WindowsMemory.hpp"

#if defined(AR_PLATFORM_OS_WINDOWS)

namespace Arcane::Windows {

	struct PageAccessMappingTableEntry {
		PageAccessFlags allocation_flags;
		DWORD protection_type;
	};

	constexpr PageAccessMappingTableEntry g_page_access_mapping[] = {
		{ PAGE_ACCESS_READ | PAGE_ACCESS_WRITE | PAGE_ACCESS_EXECUTE, PAGE_EXECUTE_READWRITE },
		{ PAGE_ACCESS_READ | PAGE_ACCESS_WRITE, PAGE_READWRITE },
		{ PAGE_ACCESS_READ | PAGE_ACCESS_EXECUTE, PAGE_EXECUTE_READ },
		{ PAGE_ACCESS_EXECUTE, PAGE_EXECUTE },
		{ PAGE_ACCESS_READ, PAGE_READONLY }
	};
	
	DWORD map_allocation_flags_to_allocation_type(PageAllocationFlags allocation_flags) {
		DWORD allocation_type = 0;
		if (allocation_flags & PAGE_ALLOCATION_PRIVATE) allocation_type = MEM_COMMIT;

		return allocation_type;
	}

	DWORD map_access_flags_to_memory_protection_type(PageAccessFlags allocation_flags) {
		for (const PageAccessMappingTableEntry& entry : g_page_access_mapping) {
			if (allocation_flags == entry.allocation_flags) return entry.protection_type;
		}

		return PAGE_NOACCESS;
	}

	B8 MemorySystem::initialize() {
		SYSTEM_INFO systemInfo = {};
		GetSystemInfo(&systemInfo);

		m_page_size = systemInfo.dwPageSize;

		return true;
	}

	void MemorySystem::shutdown() {
		
	}

	void *MemorySystem::map_memory(U64 page_count, PageAccessFlags access_flags, PageAllocationFlags allocation_flags) {
		if (page_count == 0) return nullptr;

		DWORD allocation_type = map_allocation_flags_to_allocation_type(allocation_flags);
		DWORD protection_type = map_access_flags_to_memory_protection_type(access_flags);

		LPVOID page_address = VirtualAlloc(
				NULL,
				page_count * m_page_size,
				allocation_type,
				protection_type);

		return page_address;
	}

	void MemorySystem::unmap_memory(void *pointer, U64 size) {
		if (size == 0) return;

		BOOL success = VirtualFree(
				pointer,
				size,
				MEM_RELEASE);

		static_cast<void>(success);
	}

}

#endif
