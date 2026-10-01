#pragma once

#include "./WindowsCore.hpp"
#include <Arcane/System/Memory.hpp>

#if defined(AR_PLATFORM_OS_WINDOWS)

namespace Arcane::Windows {

	class MemorySystem : public Arcane::IMemorySystem {
	public:
		B8 Initialize() override;
		void Shutdown() override;

		void *MapMemory(U64 pageCount, PageAccessFlags accessFlags, PageAllocationFlags allocationFlags) override;
		void UnmapMemory(void *Pointer, U64 Size) override;
		inline U64 GetPageSize() const override { return mPageSize; }
	
	private:
		U64 mPageSize;
	};

}

#endif
