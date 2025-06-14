#include "Mesh.hpp"

namespace Arcane {

	Mesh Mesh::Create(const GraphicsContext &context) {
		return Mesh(NativeMesh::Create(
			context.GetNativeContext()
		));
	}

	Mesh Mesh::Create(const GraphicsContext &context, const MeshData &data) {
		Ref<NativeMesh> nativeMesh = NativeMesh::Create(context.GetNativeContext());

		Buffer positions = Buffer::Create(context, data.VertexCount * sizeof(Vector3));
		Buffer normals = Buffer::Create(context, data.VertexCount * sizeof(Vector3));
		Buffer uvs = Buffer::Create(context, data.VertexCount * sizeof(Vector2));
		Buffer tangents = Buffer::Create(context, data.VertexCount * sizeof(Vector3));
		Buffer bitangents = Buffer::Create(context, data.VertexCount * sizeof(Vector3));
		Buffer indices = Buffer::Create(context, data.IndexCount * sizeof(uint32_t));

		positions.SetData(data.Positions.data());
		normals.SetData(data.Normals.data());
		uvs.SetData(data.UVs.data());
		tangents.SetData(data.Tangents.data());
		bitangents.SetData(data.Bitangents.data());
		indices.SetData(data.Indices.data());

		nativeMesh->SetVertexBuffer(0, { { InputAttribute::Position, 1, InputElementType::Vector3f32 } }, positions.GetNativeBuffer(), 0, 3 * sizeof(float));
		nativeMesh->SetVertexBuffer(1, { { InputAttribute::Normal, 1, InputElementType::Vector3f32 } }, normals.GetNativeBuffer(), 0, 3 * sizeof(float));
		nativeMesh->SetVertexBuffer(2, { { InputAttribute::UV, 1, InputElementType::Vector2f32 } }, uvs.GetNativeBuffer(), 0, 2 * sizeof(float));
		nativeMesh->SetVertexBuffer(3, { { InputAttribute::Tangent, 1, InputElementType::Vector3f32 } }, tangents.GetNativeBuffer(), 0, 3 * sizeof(float));
		nativeMesh->SetVertexBuffer(4, { { InputAttribute::Bitangent, 1, InputElementType::Vector3f32 } }, bitangents.GetNativeBuffer(), 0, 3 * sizeof(float));

		nativeMesh->SetIndexBuffer(indices.GetNativeBuffer());

		return Mesh(nativeMesh);
	}

}