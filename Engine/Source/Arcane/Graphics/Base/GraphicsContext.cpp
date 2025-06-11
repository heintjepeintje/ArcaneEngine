#include "GraphicsContext.hpp"

namespace Arcane {

	static GraphicsContext sCurrentContext;

	GraphicsContext &GraphicsContext::GetCurrent() {
		return sCurrentContext;
	}
	
	void GraphicsContext::SetCurrent(const GraphicsContext &context) {
		sCurrentContext = context;
	}

	GraphicsContext GraphicsContext::Create(const Window &window, GraphicsAPI api) {
		GraphicsContext context = GraphicsContext(NativeGraphicsContext::Create(
			window.GetNativeWindow(), api
		));
		return context;
	}

}