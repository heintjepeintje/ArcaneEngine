#include "Loader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>

#include <Arcane/Util/StringUtils.hpp>

namespace Arcane {

	MeshData LoadCube(const Vector3 &size) {
		const Vector3 halfSize = size / 2;
		
		const std::vector<Vertex> vertices = {
			// Front Face
			{ { -halfSize.x,  halfSize.y, -halfSize.z }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ { -halfSize.x, -halfSize.y, -halfSize.z }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ {  halfSize.x, -halfSize.y, -halfSize.z }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ {  halfSize.x,  halfSize.y, -halfSize.z }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

			// Right Face
			{ {  halfSize.x,  halfSize.y, -halfSize.z }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ {  halfSize.x, -halfSize.y, -halfSize.z }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ {  halfSize.x, -halfSize.y,  halfSize.z }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ {  halfSize.x,  halfSize.y,  halfSize.z }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

			// Back Face
			{ {  halfSize.x,  halfSize.y,  halfSize.z }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ {  halfSize.x, -halfSize.y,  halfSize.z }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ { -halfSize.x, -halfSize.y,  halfSize.z }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ { -halfSize.x,  halfSize.y,  halfSize.z }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

			// Left Face
			{ { -halfSize.x,  halfSize.y,  halfSize.z }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ { -halfSize.x, -halfSize.y,  halfSize.z }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ { -halfSize.x, -halfSize.y, -halfSize.z }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ { -halfSize.x,  halfSize.y, -halfSize.z }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

			// Top Face
			{ { -halfSize.x,  halfSize.y,  halfSize.z }, {  0.0f,  1.0f,  0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ { -halfSize.x,  halfSize.y, -halfSize.z }, {  0.0f,  1.0f,  0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ {  halfSize.x,  halfSize.y, -halfSize.z }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ {  halfSize.x,  halfSize.y,  halfSize.z }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },

			// Bottom Face
			{ { -halfSize.x, -halfSize.y, -halfSize.z }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ { -halfSize.x, -halfSize.y,  halfSize.z }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ {  halfSize.x, -halfSize.y,  halfSize.z }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
			{ {  halfSize.x, -halfSize.y, -halfSize.z }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		};

		const std::vector<uint32_t> indices = {
			// Front Face
			0, 1, 2,
			0, 2, 3,

			// Right Face
			4, 5, 6,
			4, 6, 7,

			// Back Face
			8, 9, 10,
			8, 10, 11,

			// Left Face
			12, 13, 14,
			12, 14, 15,

			// Top Face
			16, 17, 18,
			16, 18, 19,

			// Bottom Face
			20, 21, 22,
			20, 22, 23,
		};

		return MeshData(vertices, indices);
	}

	MeshData LoadMesh(const std::string &path) {
		MeshData data;

		std::fstream file = std::fstream(path, std::ios::in);
		AR_ASSERT(file.is_open(), "Failed to open file: %s\n", path.c_str());

		std::vector<Vector3> positions;
		std::vector<Vector3> normals;
		std::vector<Vector2> uvs;

		uint32_t index = 0;

		std::string line;
		while (std::getline(file, line)) {
			if (line.starts_with("#")) {
				continue;
			} else if (line.starts_with("v ")) {
				Vector3 position;
				sscanf(line.c_str(), "v %f %f %f", &position.x, &position.y, &position.z);
				position.z = -position.z;
				positions.push_back(position);
			} else if (line.starts_with("vn ")) {
				Vector3 normal;
				sscanf(line.c_str(), "vn %f %f %f", &normal.x, &normal.y, &normal.z);
				normal.z = -normal.z;
				normals.push_back(normal);
			} else if (line.starts_with("vt ")) {
				Vector2 uv;
				sscanf(line.c_str(), "vt %f %f", &uv.x, &uv.y);
				uvs.push_back(uv);
			} else if (line.starts_with("f")) {
				std::vector<std::string> tokens = SplitString(line, ' ');
				uint32_t vertexCount = tokens.size() - 1;
				
				if (vertexCount == 3) {
					data.Indices.push_back(index + 0);
					data.Indices.push_back(index + 1);
					data.Indices.push_back(index + 2);
				} else if (vertexCount == 4) {
					data.Indices.push_back(index + 0);
					data.Indices.push_back(index + 1);
					data.Indices.push_back(index + 2);
					data.Indices.push_back(index + 0);
					data.Indices.push_back(index + 2);
					data.Indices.push_back(index + 3);
				}
				
				for (uint32_t i = 0; i < vertexCount; i++) {
					std::vector<std::string> indices = SplitString(tokens[i + 1], '/');
					uint32_t positionIndex = std::stoul(indices[0]) - 1;
					uint32_t uvIndex = std::stoul(indices[1]) - 1;
					uint32_t normalIndex = std::stoul(indices[2]) - 1;

					Vertex vertex = {
						positions[positionIndex],
						normals[normalIndex],
						uvs[uvIndex],
						{ 1.0f, 1.0f, 1.0f, 1.0f },
					};
					data.Vertices.push_back(vertex);
				}

				index += vertexCount;
			}
		}

		return data;
	}

}