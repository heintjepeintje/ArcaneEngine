#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	struct FrameStatistics {
		uint32_t IndicesDrawn;
		uint32_t DrawCommands;
		uint32_t InstancesDrawn;
		uint64_t ElapsedCPUTime;
		uint64_t ElapsedGPUTime;
	};

}