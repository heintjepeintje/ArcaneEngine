#pragma once

#include <Arcane/Core/Platform.hpp>
#include <Arcane/System/Memory.hpp>

#if defined(AR_PLATFORM_OS_WINDOWS)
#	include <Arcane/Platform/Windows/WindowsMemory.hpp>
#endif

namespace Arcane {

	class Platform {
	public:
		IMemorySystem& memory_system() { return m_memory_system; }

	private:
#if defined(AR_PLATFORM_OS_WINDOWS)
		Windows::MemorySystem m_memory_system;
#endif
	};

}
