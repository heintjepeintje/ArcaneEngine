#pragma once

#include "GraphicsContext.hpp"
#include "Buffer.hpp"
#include <Arcane/Native/NativeMesh.hpp>
#include "Loader.hpp"

namespace Arcane {

	class Mesh {
	public:
		static Mesh Create(const GraphicsContext &context);
		static Mesh Create(const GraphicsContext &context, const MeshData &data);

	public:
		Mesh() { }
		Mesh(const Mesh &other) { mNativeMesh = other.mNativeMesh; }
		Mesh(Mesh &&other) { mNativeMesh = other.mNativeMesh; }
		~Mesh() { }

		inline void operator=(const Mesh &other) { mNativeMesh = other.mNativeMesh; }
		inline void operator=(Mesh &&other) { mNativeMesh = other.mNativeMesh; }

		inline void SetVertexBuffer(uint32_t index, const InputLayout &layout, const Buffer &vertexBuffer) { GetNativeMesh()->SetVertexBuffer(index, layout, vertexBuffer.GetNativeBuffer()); }
		inline void SetIndexBuffer(const Buffer &indexBuffer) { GetNativeMesh()->SetIndexBuffer(indexBuffer.GetNativeBuffer()); }
		inline Buffer GetVertexBuffer(uint32_t index) const { return GetNativeMesh()->GetVertexBuffer(index); }
		inline Buffer GetIndexBuffer() const { return GetNativeMesh()->GetIndexBuffer(); }

		inline InputLayout GetLayout() const { return GetNativeMesh()->GetLayout(); }

		inline Ref<NativeMesh> GetNativeMesh() const {
			AR_ASSERT(mNativeMesh, "Native mesh is invalid");
			return mNativeMesh; 
		}

	private:
		Mesh(const Ref<NativeMesh> &mesh) : mNativeMesh(mesh) { }

	private:
		Ref<NativeMesh> mNativeMesh;
	};

}