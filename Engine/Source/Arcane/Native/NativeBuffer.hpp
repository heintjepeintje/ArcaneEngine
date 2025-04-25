#pragma once

#include <Arcane/Core.hpp>
#include "NativeGraphicsContext.hpp"
#include <Arcane/Graphics/InputLayout.hpp>

namespace Arcane {

	enum class MapMode {
		None = 0,
		Read, Write, ReadWrite
	};

	enum BufferFlags {
		BufferFlag_Mutable = AR_BIT(0),
		BufferFlag_Static = AR_BIT(1),
		BufferFlag_MapRead = AR_BIT(2),
		BufferFlag_MapWrite = AR_BIT(3)
	};

	class NativeBuffer {
	public:
		static Ref<NativeBuffer> Create(const Ref<NativeGraphicsContext> &context, size_t size, uint32_t flags = 0);

	public:
		NativeBuffer() { }
		virtual ~NativeBuffer() { }

		virtual void *Map(MapMode mode) = 0;
		virtual void Unmap() = 0;
		virtual void SetData(size_t offset, size_t size, const void *data) = 0;
		virtual void Resize(size_t size) = 0;

		virtual size_t GetSize() const = 0;
	};

}