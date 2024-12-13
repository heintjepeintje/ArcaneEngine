#include "GraphicsContext.hpp"

namespace Arcane {

	GraphicsContext GraphicsContext::Create(const Window &window) {
		return GraphicsContext(NativeGraphicsContext::Create(
			window.GetNativeWindow()
		));
	}

}