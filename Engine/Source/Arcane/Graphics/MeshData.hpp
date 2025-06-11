#pragma once

#include <Arcane/Core.hpp>
#include <vector>
#include <Arcane/Math/Vector2.hpp>
#include <Arcane/Math/Vector3.hpp>

namespace Arcane {

	struct MeshData {
		std::string Name;

		bool HasPositions;
		std::vector<Vector3> Positions;
		bool HasNormals;
		std::vector<Vector3> Normals;
		bool HasUVs;
		std::vector<Vector2> UVs;
		bool HasTangents;
		std::vector<Vector3> Tangents;
		std::vector<Vector3> Bitangents;
		uint32_t VertexCount;

		bool HasIndices;
		std::vector<uint32_t> Indices;
		uint32_t IndexCount;
	};

}