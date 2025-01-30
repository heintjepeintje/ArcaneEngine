#include "GraphicsContext.hpp"

namespace Arcane {

	GraphicsContext GraphicsContext::Create(const Window &window) {
		GraphicsContext context = GraphicsContext(NativeGraphicsContext::Create(
			window.GetNativeWindow()
		));
		context.mWindow = window;
		return context;
	}

}