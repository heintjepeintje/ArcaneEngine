#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Math/Vector2.hpp>
#include <Arcane/Math/Vector3.hpp>
#include <Arcane/Data/BufferData.hpp>

namespace Arcane {

	struct MeshData {
		std::string Name;

		BufferRef Positions;
		BufferRef Normals;
		BufferRef UVs;
		BufferRef Tangents;
		BufferRef Bitangents;
		uint32_t VertexCount;

		BufferRef Indices;
		uint32_t IndexCount;
	};

}