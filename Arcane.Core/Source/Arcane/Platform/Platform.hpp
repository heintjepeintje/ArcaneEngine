#pragma once

#include <Arcane/Core/Platform.hpp>
#include <Arcane/System/Memory.hpp>

#if defined(AR_PLATFORM_OS_WINDOWS)
#	include <Arcane/Platform/Windows/WindowsMemory.hpp>
#endif

namespace Arcane {

	class Platform {
	public:
		IMemorySystem& GetMemorySystem() { return mMemorySystem; }

	private:
#if defined(AR_PLATFORM_OS_WINDOWS)
		Windows::MemorySystem mMemorySystem;
#endif
	};

}
