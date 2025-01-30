#include "Framebuffer.hpp"

namespace Arcane {

	Framebuffer Framebuffer::Create(const GraphicsContext &context, const FramebufferInfo &info) {
		return Framebuffer(NativeFramebuffer::Create(
			context.GetNativeContext(), 
			info
		));
	}

}