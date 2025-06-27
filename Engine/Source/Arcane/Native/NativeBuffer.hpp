#pragma once

#include <Arcane/Core.hpp>
#include "NativeGraphicsContext.hpp"
#include <Arcane/Graphics/InputLayout.hpp>

namespace Arcane {

	enum class MapMode {
		None = 0,
		Read, Write, ReadWrite
	};

	namespace BufferFlag {
		enum _BufferFlags {
			Mutable = AR_BIT(0),
			Static = AR_BIT(1),
			MapRead = AR_BIT(2),
			MapWrite = AR_BIT(3)
		};
	}

	class NativeBuffer {
	public:
		static Ref<NativeBuffer> Create(const Ref<NativeGraphicsContext> &context, size_t size, uint32_t flags);

	public:
		NativeBuffer() { }
		virtual ~NativeBuffer() { }

		virtual void *Map(MapMode mode) = 0;
		virtual void Unmap() = 0;
		virtual void SetData(size_t offset, size_t size, const void *data) = 0;
		virtual void Resize(size_t size) = 0;

		virtual size_t GetSize() const = 0;

		virtual void Destroy() = 0;
		virtual bool IsValid() const = 0;
	};

}