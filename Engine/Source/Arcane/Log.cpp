#include "Log.hpp"

#include <cstdarg>
#include <cstdio>
#include <format>

namespace Arcane {

	Logger &GetEngineLogger() {
		static Logger logger("Arcane");
		return logger;
	}

}