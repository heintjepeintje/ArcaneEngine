#include "Buffer.hpp"

namespace Arcane {

	Buffer Buffer::Create(const GraphicsContext &context, size_t size) {
		return Buffer(NativeBuffer::Create(context.GetNativeContext(), size));
	}

}