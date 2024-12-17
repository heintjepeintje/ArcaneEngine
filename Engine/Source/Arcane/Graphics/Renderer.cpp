#include "Renderer.hpp"

namespace Arcane {

	RendererAPI RendererAPI::Create(const GraphicsContext &context) {
		return RendererAPI(NativeRendererAPI::Create(
			context.GetNativeContext()
		));
	}

}