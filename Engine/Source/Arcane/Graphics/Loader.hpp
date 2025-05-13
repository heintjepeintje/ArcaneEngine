#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Math/Vector3.hpp>
#include "Image.hpp"
#include "Color.hpp"

#include <vector>
#include <functional>
#include <array>

namespace Arcane {

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
		uint32_t IndexWidth;
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
	
	void ProcessImage(ImageData &data, ImageProcess process);

}