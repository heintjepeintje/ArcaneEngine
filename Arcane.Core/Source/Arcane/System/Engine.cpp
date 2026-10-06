#include "./Engine.hpp"

namespace Arcane {

	B8 Engine::initialize() {
		B8 is_memory_system_initialized = m_platform.memory_system().initialize();
		if (!is_memory_system_initialized) return false;

		for (U32 i = 0; i < HEAP_COUNT; i++) {
			m_memory_heaps[i] = MemoryHeap(
				m_platform.memory_system(),
				g_engine_heap_data[i].minimum_size,
				g_engine_heap_data[i].flags);
		}

		return true;
	}
	
	void Engine::shutdown() {
		m_platform.memory_system().shutdown();
	}

}
