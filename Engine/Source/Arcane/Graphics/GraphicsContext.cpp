#include "GraphicsContext.hpp"

namespace Arcane {

	static GraphicsContext sCurrentContext;

	GraphicsContext &GraphicsContext::GetCurrent() {
		return sCurrentContext;
	}
	
	void GraphicsContext::SetCurrent(const GraphicsContext &context) {
		sCurrentContext = context;
	}

	GraphicsContext GraphicsContext::Create(const Window &window) {
		GraphicsContext context = GraphicsContext(NativeGraphicsContext::Create(
			window.GetNativeWindow()
		));
		context.mWindow = window;
		return context;
	}

}