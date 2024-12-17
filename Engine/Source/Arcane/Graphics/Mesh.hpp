#pragma once

#include "GraphicsContext.hpp"
#include "Buffer.hpp"
#include <Arcane/Native/NativeMesh.hpp>

namespace Arcane {

	class Mesh {
	public:
		static Mesh Create(const GraphicsContext &context);

	public:
		Mesh() { }
		~Mesh() { }

		inline void SetVertexBuffer(uint32_t index, const InputLayout &layout, const Buffer &vertexBuffer) { mNativeMesh->SetVertexBuffer(index, layout, vertexBuffer.GetNativeBuffer()); }
		inline void SetIndexBuffer(const Buffer &indexBuffer) { mNativeMesh->SetIndexBuffer(indexBuffer.GetNativeBuffer()); }

		inline InputLayout GetLayout() const { return mNativeMesh->GetLayout(); }

		inline std::shared_ptr<NativeMesh> GetNativeMesh() const { return mNativeMesh; }

	private:
		Mesh(const std::shared_ptr<NativeMesh> &mesh) : mNativeMesh(mesh) { }

	private:
		std::shared_ptr<NativeMesh> mNativeMesh;
	};

}