#include "Mesh.hpp"

namespace Arcane {

	Mesh Mesh::Create(const GraphicsContext &context) {
		return Mesh(NativeMesh::Create(
			context.GetNativeContext()
		));
	}

	Mesh Mesh::Create(const GraphicsContext &context, const MeshData &data) {
		Ref<NativeMesh> nativeMesh = NativeMesh::Create(context.GetNativeContext());

		Buffer positions = 	Buffer::Create(context, data.Positions.GetSize());
		Buffer normals = 	Buffer::Create(context, data.Normals.GetSize());
		Buffer uvs = 		Buffer::Create(context, data.UVs.GetSize());
		Buffer tangents =	Buffer::Create(context, data.Tangents.GetSize());
		Buffer bitangents = Buffer::Create(context, data.Bitangents.GetSize());
		Buffer indices = 	Buffer::Create(context, data.Indices.GetSize());

		positions.SetData(data.Positions);
		normals.SetData(data.Normals);
		uvs.SetData(data.UVs);
		tangents.SetData(data.Tangents);
		bitangents.SetData(data.Bitangents);
		indices.SetData(data.Indices);

		nativeMesh->SetVertexBuffer(0, { { InputAttribute::Position, 1, InputElementType::Vector3f32 } }, positions.GetNativeBuffer(), 0, 3 * sizeof(float));
		nativeMesh->SetVertexBuffer(1, { { InputAttribute::Normal, 1, InputElementType::Vector3f32 } }, normals.GetNativeBuffer(), 0, 3 * sizeof(float));
		nativeMesh->SetVertexBuffer(2, { { InputAttribute::UV, 1, InputElementType::Vector2f32 } }, uvs.GetNativeBuffer(), 0, 2 * sizeof(float));
		nativeMesh->SetVertexBuffer(3, { { InputAttribute::Tangent, 1, InputElementType::Vector3f32 } }, tangents.GetNativeBuffer(), 0, 3 * sizeof(float));
		nativeMesh->SetVertexBuffer(4, { { InputAttribute::Bitangent, 1, InputElementType::Vector3f32 } }, bitangents.GetNativeBuffer(), 0, 3 * sizeof(float));

		nativeMesh->SetIndexBuffer(indices.GetNativeBuffer());

		return Mesh(nativeMesh);
	}

}