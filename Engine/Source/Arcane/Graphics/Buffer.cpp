#include "Buffer.hpp"

namespace Arcane {

	Buffer Buffer::Create(const GraphicsContext &context, BufferType type, size_t size) {
		return Buffer(NativeBuffer::Create(context.GetNativeContext(), type, size));
	}

}