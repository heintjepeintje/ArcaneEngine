#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	void *_Allocate(size_t size);
	void *_ReAllocate(void *ptr, size_t size);
	void _Free(void *ptr);

	void *_CopyMemory(void *dest, const void *src, size_t size);
	void *_MoveMemory(void *dest, void *src, size_t size);
	void *_SetMemory(void *ptr, int value, size_t size);

}