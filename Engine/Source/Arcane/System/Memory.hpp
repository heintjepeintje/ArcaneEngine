#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeMemory.hpp>

namespace Arcane {

	void *Allocate(size_t size) { return _Allocate(size); }
	void *ReAllocate(void *ptr, size_t size) { return _ReAllocate(ptr, size); }
	void Free(void *ptr) { _Free(ptr); }

	template<typename _Type>
	_Type *AllocateArray(size_t count) { return _Allocate(count * sizeof(_Type)); }
	template<typename _Type>
	_Type *ReAllocateArray(_Type *ptr, size_t count) { return _ReAllocate(ptr, count * sizeof(_Type)); }

	template<typename _Type>
	_Type *AllocateObject() { return (_Type*)_Allocate(sizeof(_Type)); }

	void *CopyMemory(void *dest, const void *src, size_t size) { return _CopyMemory(dest, src, size); }
	void *MoveMemory(void *dest, void *src, size_t size) { return _MoveMemory(dest, src, size); }
	void *SetMemory(void *ptr, int value, size_t size) { return _SetMemory(ptr, value, size); }
	void *ZeroMemory(void *ptr, size_t size) { return _SetMemory(ptr, 0, size); }

}