#include "WindowsCore.hpp"

namespace Arcane {

	Logger &GetWindowsLogger() {
		static Logger logger("Arcane.Windows");
		return logger;
	}

}