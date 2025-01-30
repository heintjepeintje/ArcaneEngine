#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/InputLayout.hpp>
#include "NativeGraphicsContext.hpp"
#include "NativeBuffer.hpp"

namespace Arcane {

	class NativeMesh {
	public:
		static Ref<NativeMesh> Create(const Ref<NativeGraphicsContext> &context);

	public:
		NativeMesh() { }
		virtual ~NativeMesh() { }

		virtual void SetVertexBuffer(uint32_t index, const InputLayout &layout, const Ref<NativeBuffer> &vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<NativeBuffer> &indexBuffer) = 0;
		virtual uint32_t GetIndexCount() const = 0;

		virtual InputLayout GetLayout() const = 0;
	};

}