#include <Arcane/Native/NativeTime.hpp>

#include <profileapi.h>

namespace Arcane {

	static uint64_t GetPerformanceFrequency() {
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		return frequency.QuadPart;
	}

	static uint64_t sFrequency = GetPerformanceFrequency();

	uint64_t _GetCurrentTimeMillis() {
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		
		return (uint64_t)((double)counter.QuadPart / (double)sFrequency * 1000.0);
	}

}