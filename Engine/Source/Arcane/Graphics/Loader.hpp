#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Physics/AABB.hpp>
#include "Image.hpp"
#include "Color.hpp"

#include <vector>
#include <functional>
#include <array>

namespace Arcane {

	enum class MeshProcess {
		None = 0,
		MoveOriginToCenter,
		NormalizeMesh,
		GenerateNormals,
		GenerateUVs,
		GenerateTangents,
		GenerateBoundingBox,
		FlipUVs,
		RemoveDuplicateVertices,
		RemoveUnusedVertices,
	};

	enum class ImageProcess {
		None = 0,
		FlipHorizontal,
		FlipVertical,
		Invert,
	};

	struct MeshData {
		std::vector<Vector3> Positions;
		std::vector<Vector3> Normals;
		std::vector<Vector2> UVs;
		std::vector<Vector3> Tangents;
		std::vector<Vector3> Bitangents;
		std::vector<uint32_t> Indices;

		AABB BoundingBox;
	};

	struct ImageData {
		uint32_t Width, Height;
		ImageFormat Format;
		uint8_t *Data;
	};

	MeshData LoadCube(const Vector3 &size = Vector3(1.0f), bool scaleUVs = true);
	MeshData LoadMesh(const std::string &path);
	ImageData LoadImage(const std::string &path, ImageFormat requestedFormat);
	ImageData LoadImage(const Color &color, ImageFormat format);

	void ProcessMesh(MeshData &data, MeshProcess process);
	void ProcessImage(ImageData &data, ImageProcess process);

}