#include "VulkanCore.hpp"

namespace Arcane {

	Logger &GetVulkanLogger() {
		static Logger logger("Arcane.Vulkan");
		return logger;
	}

} // namespace Arcane