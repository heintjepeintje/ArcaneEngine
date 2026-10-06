#pragma once

#include "./WindowsCore.hpp"
#include <Arcane/System/Memory.hpp>

#if defined(AR_PLATFORM_OS_WINDOWS)

namespace Arcane::Windows {

	class MemorySystem : public Arcane::IMemorySystem {
	public:
		B8 initialize() override;
		void shutdown() override;

		void *map_memory(U64 pageCount, PageAccessFlags accessFlags, PageAllocationFlags allocationFlags) override;
		void unmap_memory(void *Pointer, U64 Size) override;
		inline U64 page_size() const override { return m_page_size; }
	
	private:
		U64 m_page_size;
	};

}

#endif
