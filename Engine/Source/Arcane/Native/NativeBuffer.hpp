#pragma once

#include <Arcane/Core.hpp>
#include "NativeGraphicsContext.hpp"
#include <Arcane/Graphics/InputLayout.hpp>

namespace Arcane {

	enum class BufferType {
		None = 0,
		Vertex, Index, Uniform
	};

	enum class MapMode {
		None = 0,
		Read, Write, ReadWrite
	};

	class NativeBuffer {
	public:
		static std::shared_ptr<NativeBuffer> Create(const std::shared_ptr<NativeGraphicsContext> &context, BufferType type, size_t size);
		static std::shared_ptr<NativeBuffer> Create(const std::shared_ptr<NativeGraphicsContext> &context, BufferType type, const InputLayout &layout, size_t size);

	public:
		NativeBuffer() { }
		virtual ~NativeBuffer() { }

		virtual void *Map(MapMode mode) = 0;
		virtual void Unmap() = 0;

		virtual size_t GetSize() const = 0;
		virtual BufferType GetType() const = 0;
	};

}