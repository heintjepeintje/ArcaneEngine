#pragma once

#include <Arcane/Platform/Platform.hpp>
#include <Arcane/Core/Units.hpp>
#include "./System.hpp"
#include "./Memory.hpp"

namespace Arcane {

	struct EngineHeapData {
		U32 minimum_size;
		MemoryHeapFlags flags;
	};

	enum MemoryHeapIndices {
		HEAP_INDEX_SYSTEM_HEAP,
		HEAP_INDEX_GRAPHICS_HEAP,
		HEAP_INDEX_PHYSICS_HEAP,
		HEAP_INDEX_MISCELLANIOUS_HEAP,

		HEAP_COUNT
	};

	constexpr static EngineHeapData g_engine_heap_data[] = {
		{ from_kibibytes(8), MEMORY_HEAP_DEFRAGMENT_ON_DEALLOCATE | MEMORY_HEAP_ZERO_ON_DEALLOCATE },
		{ from_kibibytes(16), MEMORY_HEAP_DEFRAGMENT_ON_DEALLOCATE },
		{ from_kibibytes(8), MEMORY_HEAP_DEFRAGMENT_ON_DEALLOCATE },
		{ from_kibibytes(8), MEMORY_HEAP_DEFRAGMENT_ON_DEALLOCATE }
	};

	using MemoryHeapIndex = U32;

	class Engine : public ISystem {
	public:
		Engine() = default;
		~Engine() = default;

		B8 initialize() override;
		void shutdown() override;

		inline Platform& platform() { return m_platform; }

		inline MemoryHeap& find_heap(MemoryHeapIndex heap) { return m_memory_heaps[heap]; }
	private:
		Platform m_platform;

		MemoryHeap m_memory_heaps[HEAP_COUNT];
	};

}
