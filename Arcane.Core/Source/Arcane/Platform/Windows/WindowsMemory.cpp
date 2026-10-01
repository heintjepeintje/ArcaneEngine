#include "./WindowsMemory.hpp"

#if defined(AR_PLATFORM_OS_WINDOWS)

namespace Arcane::Windows {

	struct PageAccessMappingTableEntry {
		PageAccessFlags allocationFlags;
		DWORD protectionType;
	};

	constexpr PageAccessMappingTableEntry gPageAccessMapping[] = {
		{ PAGE_ACCESS_READ | PAGE_ACCESS_WRITE | PAGE_ACCESS_EXECUTE, PAGE_EXECUTE_READWRITE },
		{ PAGE_ACCESS_READ | PAGE_ACCESS_WRITE, PAGE_READWRITE },
		{ PAGE_ACCESS_READ | PAGE_ACCESS_EXECUTE, PAGE_EXECUTE_READ },
		{ PAGE_ACCESS_EXECUTE, PAGE_EXECUTE },
		{ PAGE_ACCESS_READ, PAGE_READONLY }
	};
	
	DWORD MapAllocationFlagsToAllocationType(PageAllocationFlags allocationFlags) {
		DWORD allocationType = 0;
		if (allocationFlags & PAGE_ALLOCATION_PRIVATE) allocationType = MEM_COMMIT;

		return allocationType; 
	}

	DWORD MapAccessFlagsToMemoryProtectionType(PageAccessFlags allocationFlags) {
		for (const PageAccessMappingTableEntry& entry : gPageAccessMapping) {
			if (allocationFlags == entry.allocationFlags) return entry.protectionType;
		}

		return PAGE_NOACCESS;
	}

	B8 MemorySystem::Initialize() {
		SYSTEM_INFO systemInfo = {};
		GetSystemInfo(&systemInfo);

		mPageSize = systemInfo.dwPageSize;

		return true;
	}

	void MemorySystem::Shutdown() {
		
	}

	void *MemorySystem::MapMemory(U64 pageCount, PageAccessFlags accessFlags, PageAllocationFlags allocationFlags) {
		if (pageCount == 0) return nullptr;

		DWORD allocationType = MapAllocationFlagsToAllocationType(allocationFlags);
		DWORD protectionType = MapAccessFlagsToMemoryProtectionType(accessFlags);

		LPVOID pageAddress = VirtualAlloc(
				NULL,
				pageCount * mPageSize,
				allocationType,
				protectionType);

		return pageAddress;
	}

	void MemorySystem::UnmapMemory(void *pointer, U64 size) {
		if (size == 0) return;

		BOOL success = VirtualFree(
				pointer,
				size,
				MEM_RELEASE);

		static_cast<void>(success);
	}

}

#endif
