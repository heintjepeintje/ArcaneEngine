#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Math/Vector.hpp>

#include <vector>

namespace Arcane {

	struct Vertex {
		Vertex(const Vector3 &position, const Vector3 &normal, const Vector2 &uv, const Vector4 &color) : Position(position), Normal(normal), UV(uv), Color(color) { }

		Vector3 Position;
		Vector3 Normal;
		Vector2 UV;
		Vector4 Color;
	};

	struct MeshData {
		std::vector<Vertex> Vertices;
		std::vector<uint32_t> Indices;
	};

	MeshData LoadCube(const Vector3 &size = Vector3(1.0f));
	MeshData LoadMesh(const std::string &path);

}