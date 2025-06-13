#include "Buffer.hpp"

namespace Arcane {

	Buffer Buffer::Create(const GraphicsContext &context, size_t size, uint32_t flags) {
		return Buffer(NativeBuffer::Create(context.GetNativeContext(), size, flags));
	}

}