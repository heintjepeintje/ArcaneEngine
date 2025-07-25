#pragma once

#include <Arcane/Core.hpp>
#include "GraphicsContext.hpp"
#include <Arcane/Native/NativeBuffer.hpp>
#include <Arcane/Data/BufferData.hpp>

namespace Arcane {

	class Buffer {
	friend class Mesh;
	public:
		static Buffer Create(const GraphicsContext &context, size_t size, uint32_t flags = 0);

	public:
		Buffer() { }
		~Buffer() { }

		inline void *Map(MapMode mode) { return GetNativeBuffer()->Map(mode); }

		template<typename _Type>
		inline _Type *Map(MapMode mode) { return (_Type*)GetNativeBuffer()->Map(mode); }
		template<typename _Type>
		inline const _Type *Map(MapMode mode) const { return (const _Type*)GetNativeBuffer()->Map(mode); }
		
		inline void Unmap() { GetNativeBuffer()->Unmap(); }

		inline void SetData(size_t offset, size_t size, const void *data) { GetNativeBuffer()->SetData(offset, size, data); }
		inline void SetData(size_t size, const void *data) { GetNativeBuffer()->SetData(0, size, data); }
		inline void SetData(const void *data) { GetNativeBuffer()->SetData(0, mNativeBuffer->GetSize(), data); }
		inline void SetData(const BufferRef &data) { GetNativeBuffer()->SetData(0, data.GetSize(), data.GetPointer()); }
		inline void SetData(size_t offset, const BufferRef &data) { GetNativeBuffer()->SetData(offset, data.GetSize(), data.GetPointer()); }

		template<typename _Type>
		inline void SetData(const _Type &data) { GetNativeBuffer()->SetData(0, sizeof(_Type), &data); }
		
		inline void Resize(size_t size) { GetNativeBuffer()->Resize(size); }
		inline size_t GetSize() const { return GetNativeBuffer()->GetSize(); }

		inline Ref<NativeBuffer> GetNativeBuffer() const {
			AR_ASSERT(mNativeBuffer, "Native buffer is invalid");
			return mNativeBuffer;
		}

	private:
		Buffer(const Ref<NativeBuffer> &buffer) : mNativeBuffer(buffer) { }

	private:
		Ref<NativeBuffer> mNativeBuffer;
	};

}