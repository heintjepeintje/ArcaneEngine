#pragma once

#include <Arcane/Core.hpp>
#include "NativeGraphicsContext.hpp"
#include <Arcane/Graphics/InputLayout.hpp>

namespace Arcane {

	enum class MapMode {
		None = 0,
		Read, Write, ReadWrite
	};

	class NativeBuffer {
	public:
		static Ref<NativeBuffer> Create(const Ref<NativeGraphicsContext> &context, size_t size);

	public:
		NativeBuffer() { }
		virtual ~NativeBuffer() { }

		virtual void *Map(MapMode mode) = 0;
		virtual void Unmap() = 0;
		virtual void SetData(size_t offset, size_t size, const void *data) = 0;

		virtual size_t GetSize() const = 0;
	};

}