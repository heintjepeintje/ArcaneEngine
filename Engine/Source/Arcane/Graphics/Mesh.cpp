#include "Mesh.hpp"

namespace Arcane {

	Mesh Mesh::Create(const GraphicsContext &context) {
		return Mesh(NativeMesh::Create(
			context.GetNativeContext()
		));
	}

	Mesh Mesh::Create(const GraphicsContext &context, const MeshData &data) {
		Ref<NativeMesh> nativeMesh = NativeMesh::Create(context.GetNativeContext());

		Buffer positions = Buffer::Create(context, data.Positions.size() * sizeof(Vector3));
		Buffer normals = Buffer::Create(context, data.Normals.size() * sizeof(Vector3));
		Buffer uvs = Buffer::Create(context, data.UVs.size() * sizeof(Vector2));
		Buffer tangents = Buffer::Create(context, data.Tangents.size() * sizeof(Vector3));
		Buffer bitangents = Buffer::Create(context, data.Bitangents.size() * sizeof(Vector3));
		Buffer indices = Buffer::Create(context, data.Indices.size() * sizeof(uint32_t));

		positions.SetData(data.Positions.data());
		normals.SetData(data.Normals.data());
		uvs.SetData(data.UVs.data());
		tangents.SetData(data.Tangents.data());
		bitangents.SetData(data.Bitangents.data());

		indices.SetData(data.Indices.data());

		nativeMesh->SetVertexBuffer(0, { { InputAttribute::Position, 1, InputElementType::Vector3 } }, positions.GetNativeBuffer());
		nativeMesh->SetVertexBuffer(1, { { InputAttribute::Normal, 1, InputElementType::Vector3 } }, normals.GetNativeBuffer());
		nativeMesh->SetVertexBuffer(2, { { InputAttribute::UV, 1, InputElementType::Vector2 } }, uvs.GetNativeBuffer());
		nativeMesh->SetVertexBuffer(3, { { InputAttribute::Tangent, 1, InputElementType::Vector3 } }, tangents.GetNativeBuffer());
		nativeMesh->SetVertexBuffer(4, { { InputAttribute::Bitangent, 1, InputElementType::Vector3 } }, bitangents.GetNativeBuffer());

		nativeMesh->SetIndexBuffer(indices.GetNativeBuffer());

		return Mesh(nativeMesh);
	}

}