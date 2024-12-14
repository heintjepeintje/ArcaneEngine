#pragma once

#include <Arcane/Core.hpp>
#include "GraphicsContext.hpp"
#include <Arcane/Native/NativeBuffer.hpp>

namespace Arcane {

	class Buffer {
	public:
		static Buffer Create(const GraphicsContext &context, BufferType type, size_t size);

	public:
		Buffer() { }
		~Buffer() { }

		inline void *Map(MapMode mode) { return mNativeBuffer->Map(mode); }
		inline void Unmap() { mNativeBuffer->Unmap(); }

		inline size_t GetSize() const { return mNativeBuffer->GetSize(); }
		inline BufferType GetType() const { return mNativeBuffer->GetType(); }

		inline std::shared_ptr<NativeBuffer> GetNativeBuffer() const { return mNativeBuffer; }

	private:
		Buffer(const std::shared_ptr<NativeBuffer> &buffer) : mNativeBuffer(buffer) { }

	private:
		std::shared_ptr<NativeBuffer> mNativeBuffer;
	};

}