#include <Arcane/Native/NativeMemory.hpp>

#include "WindowsCore.hpp"

namespace Arcane {

	void *_Allocate(size_t size) {
		AR_ASSERT(size > 0, "Cannot allocate zero bytes");

		HANDLE processHeap = GetProcessHeap();
		AR_WINDOWS_ASSERT(processHeap != INVALID_HANDLE_VALUE, "Failed to get process heap");

		void *ptr = HeapAlloc(processHeap, HEAP_ZERO_MEMORY, size);
		AR_WINDOWS_ASSERT(ptr != nullptr, "Failed to allocate memory: {}", GetWindowsErrorMessageString(GetLastError()));

		TracyAlloc(ptr, size);

		return ptr;
	}

	void *_ReAllocate(void *ptr, size_t size) {
		AR_ASSERT(size > 0, "Cannot reallocate to zero bytes");

		HANDLE processHeap = GetProcessHeap();
		AR_WINDOWS_ASSERT(processHeap != INVALID_HANDLE_VALUE, "Failed to get process heap");

		void *newPtr = HeapReAlloc(processHeap, HEAP_ZERO_MEMORY, ptr, size);
		AR_WINDOWS_ASSERT(newPtr != nullptr, "Failed to reallocate memory: {}", GetWindowsErrorMessageString(GetLastError()));

		TracyFree(ptr);
		TracyAlloc(newPtr, size);

		return newPtr;
	}

	void _Free(void *ptr) {
		AR_ASSERT(ptr != nullptr, "Cannot free null pointer");

		HANDLE processHeap = GetProcessHeap();
		AR_WINDOWS_ASSERT(processHeap != INVALID_HANDLE_VALUE, "Failed to get process heap");

		BOOL result = HeapFree(processHeap, 0, ptr);
		AR_WINDOWS_ASSERT(result, "Failed to free memory: {}", GetWindowsErrorMessageString(GetLastError()));

		TracyFree(ptr);
	}

	void *_CopyMemory(void *dest, const void *src, size_t size) {
		AR_ASSERT(dest != nullptr && src != nullptr, "Cannot copy memory to/from null pointer");
		AR_ASSERT(size > 0, "Cannot copy zero bytes");

		return std::memcpy(dest, src, size);
	}

	void *_MoveMemory(void *dest, void *src, size_t size) {
		AR_ASSERT(dest != nullptr && src != nullptr, "Cannot move memory to/from null pointer");
		AR_ASSERT(size > 0, "Cannot move zero bytes");

		return std::memmove(dest, src, size);
	}

	void *_SetMemory(void *ptr, int value, size_t size) {
		AR_ASSERT(ptr != nullptr, "Cannot set memory of null pointer");
		AR_ASSERT(size > 0, "Cannot set zero bytes");

		return std::memset(ptr, value, size);
	}
	
}