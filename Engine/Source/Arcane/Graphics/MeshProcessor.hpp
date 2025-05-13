#pragma once

#include "Loader.hpp"
#include <vector>
#include <initializer_list>

namespace Arcane {

	enum class MeshProcess {
		None = 0,
		MoveOriginToCenter,
		Normalize,
		GenerateNormals,
		GenerateUVs,
		GenerateTangents,
		SwapWindingOrder,
		FlipUVs,
		RemoveDuplicateVertices,
		RemoveUnusedVertices,
	};

	struct Triangle {
		uint32_t Indices[3];
	};

	class MeshProcessor {
	public:
		MeshProcessor();
		MeshProcessor(const std::initializer_list<MeshProcess> &processes);
		~MeshProcessor();

		MeshData &Process(MeshData &data) const;

	private:
		std::vector<MeshProcess> mProcesses;
	};

}