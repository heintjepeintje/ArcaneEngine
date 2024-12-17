#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/InputLayout.hpp>
#include "NativeGraphicsContext.hpp"
#include "NativeBuffer.hpp"

namespace Arcane {

	class NativeMesh {
	public:
		static std::shared_ptr<NativeMesh> Create(const std::shared_ptr<NativeGraphicsContext> &context);

	public:
		NativeMesh() { }
		virtual ~NativeMesh() { }

		virtual void SetVertexBuffer(uint32_t index, const InputLayout &layout, const std::shared_ptr<NativeBuffer> &vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<NativeBuffer> &indexBuffer) = 0;

		virtual InputLayout GetLayout() const = 0;
	};

}