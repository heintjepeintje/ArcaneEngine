#include "OpenGLCore.hpp"

namespace Arcane {

	Logger &GetOpenGLLogger() {
		static Logger logger("Arcane.OpenGL");
		return logger;
	}

}