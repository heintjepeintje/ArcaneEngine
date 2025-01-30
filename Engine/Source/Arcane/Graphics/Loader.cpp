#include "Loader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>

#include <Arcane/Util/StringUtils.hpp>
#include <Arcane/Math/Math.hpp>

#include <Arcane/Util/ByteUtil.hpp>

namespace Arcane {

	MeshData LoadCube(const Vector3 &size, bool scaleUVs) {
		const Vector3 halfSize = size / 2.0f;

		const std::vector<Vector3> positions = {
			{  halfSize.X,  halfSize.Y, -halfSize.Z }, 
			{ -halfSize.X,  halfSize.Y, -halfSize.Z }, 
			{ -halfSize.X, -halfSize.Y, -halfSize.Z }, 
			{  halfSize.X, -halfSize.Y, -halfSize.Z }, 

			{  halfSize.X,  halfSize.Y,  halfSize.Z },
			{  halfSize.X,  halfSize.Y, -halfSize.Z },
			{  halfSize.X, -halfSize.Y, -halfSize.Z },
			{  halfSize.X, -halfSize.Y,  halfSize.Z },

			{ -halfSize.X,  halfSize.Y,  halfSize.Z },
			{  halfSize.X,  halfSize.Y,  halfSize.Z },
			{  halfSize.X, -halfSize.Y,  halfSize.Z },
			{ -halfSize.X, -halfSize.Y,  halfSize.Z },

			{ -halfSize.X,  halfSize.Y, -halfSize.Z },
			{ -halfSize.X,  halfSize.Y,  halfSize.Z },
			{ -halfSize.X, -halfSize.Y,  halfSize.Z },
			{ -halfSize.X, -halfSize.Y, -halfSize.Z },

			{  halfSize.X,  halfSize.Y,  halfSize.Z },
			{ -halfSize.X,  halfSize.Y,  halfSize.Z },
			{ -halfSize.X,  halfSize.Y, -halfSize.Z },
			{  halfSize.X,  halfSize.Y, -halfSize.Z },

			{  halfSize.X, -halfSize.Y, -halfSize.Z },
			{ -halfSize.X, -halfSize.Y, -halfSize.Z },
			{ -halfSize.X, -halfSize.Y,  halfSize.Z },
			{  halfSize.X, -halfSize.Y,  halfSize.Z },
		};

		std::vector<Vector3> normals = {
			{  0.0f,  0.0f,  -1.0f },
			{  0.0f,  0.0f,  -1.0f },
			{  0.0f,  0.0f,  -1.0f },
			{  0.0f,  0.0f,  -1.0f },

			{  1.0f,  0.0f,   0.0f },
			{  1.0f,  0.0f,   0.0f },
			{  1.0f,  0.0f,   0.0f },
			{  1.0f,  0.0f,   0.0f },

			{  0.0f,  0.0f,   1.0f },
			{  0.0f,  0.0f,   1.0f },
			{  0.0f,  0.0f,   1.0f },
			{  0.0f,  0.0f,   1.0f },

			{ -1.0f,  0.0f,   0.0f },
			{ -1.0f,  0.0f,   0.0f },
			{ -1.0f,  0.0f,   0.0f },
			{ -1.0f,  0.0f,   0.0f },

			{  0.0f,  1.0f,   0.0f },
			{  0.0f,  1.0f,   0.0f },
			{  0.0f,  1.0f,   0.0f },
			{  0.0f,  1.0f,   0.0f },
			
			{  0.0f, -1.0f,   0.0f },
			{  0.0f, -1.0f,   0.0f },
			{  0.0f, -1.0f,   0.0f },
			{  0.0f, -1.0f,   0.0f },
		};

		std::vector<Vector2> uvs;
		if (scaleUVs) {
			uvs = std::vector<Vector2>({
				{ 1.0f, 1.0f },
				{ 0.0f, 1.0f },
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },

				{ 1.0f, 1.0f },
				{ 0.0f, 1.0f },
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },

				{ 1.0f, 1.0f },
				{ 0.0f, 1.0f },
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },

				{ 1.0f, 1.0f },
				{ 0.0f, 1.0f },
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },

				{ 1.0f, 1.0f },
				{ 0.0f, 1.0f },
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },

				{ 1.0f, 1.0f },
				{ 0.0f, 1.0f },
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },
			});
		} else {
			uvs = std::vector<Vector2>({
				{ size.X, size.Y },
				{ 0.0f, size.Y },
				{ 0.0f, 0.0f },
				{ size.X, 0.0f },

				{ size.Z, size.Y },
				{ 0.0f, size.Y },
				{ 0.0f, 0.0f },
				{ size.Z, 0.0f },

				{ size.X, size.Y },
				{ 0.0f, size.Y },
				{ 0.0f, 0.0f },
				{ size.X, 0.0f },

				{ size.Z, size.Y },
				{ 0.0f, size.Y },
				{ 0.0f, 0.0f },
				{ size.Z, 0.0f },

				{ size.X, size.Z },
				{ 0.0f, size.Z },
				{ 0.0f, 0.0f },
				{ size.X, 0.0f },

				{ size.X, size.Z },
				{ 0.0f, size.Z },
				{ 0.0f, 0.0f },
				{ size.X, 0.0f },
			});
		}

		const std::vector<uint32_t> indices = {
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7,

			8, 9, 10,
			8, 10, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,
			
			20, 21, 22,
			20, 22, 23,
		};

		std::vector<Vector3> tangents(positions.size(), Vector3(0.0f));
		std::vector<Vector3> bitangents(positions.size(), Vector3(0.0f));

		return MeshData(positions, normals, uvs, tangents, bitangents, indices);
	}

	MeshData LoadMesh(const std::string &path) {
		std::fstream file = std::fstream(path, std::ios::in);
		AR_ASSERT(file.is_open(), "Failed to open file: %s\n", path.c_str());

		uint32_t positionCount = 0;
		uint32_t normalCount = 0;
		uint32_t uvCount = 0;

		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;

		std::string line;
		while (std::getline(file, line)) {
			if (line.starts_with("#")) {
				continue;
			} else if (line.starts_with("v ")) {
				positionCount++;
			} else if (line.starts_with("vn ")) {
				normalCount++;
			} else if (line.starts_with("vt ")) {
				uvCount++;
			} else if (line.starts_with("f")) {
				uint32_t count = SplitString(line, ' ').size() - 1;
				vertexCount += count;

				if (count == 3) {
					indexCount += 3;
				} else if (count == 4) {
					indexCount += 6;
				}

			}
		}

		std::vector<Vector3> uniquePositions;
		uniquePositions.resize(positionCount);
		std::vector<Vector3> uniqueNormals;
		uniqueNormals.resize(normalCount);
		std::vector<Vector2> uniqueUVs;
		uniqueUVs.resize(uvCount);
		std::vector<uint32_t> indices;
		indices.resize(indexCount);

		std::vector<Vector3> positions;
		positions.resize(vertexCount);
		std::vector<Vector3> normals;
		normals.resize(vertexCount);
		std::vector<Vector2> uvs;
		uvs.resize(vertexCount);
		std::vector<Vector3> tangents;
		tangents.resize(vertexCount);
		std::vector<Vector3> bitangents;
		bitangents.resize(vertexCount);

		uint32_t index = 0;

		while (std::getline(file, line)) {
			if (line.starts_with("#")) {
				continue;
			} else if (line.starts_with("v ")) {
				float x, y, z;
				sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
				uniquePositions.emplace_back(x, y, z);
			} else if (line.starts_with("vn ")) {
				float x, y, z;
				sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
				uniqueNormals.emplace_back(x, y, z);
			} else if (line.starts_with("vt ")) {
				float x, y;
				sscanf(line.c_str(), "vt %f %f", &x, &y);
				uniqueUVs.emplace_back(x, y);
			} else if (line.starts_with("f")) {
				std::vector<std::string> tokens = SplitString(line, ' ');
				uint32_t vertexCount = tokens.size() - 1;
				
				if (vertexCount == 3) {
					indices.push_back(index + 0);
					indices.push_back(index + 1);
					indices.push_back(index + 2);
				} else if (vertexCount == 4) {
					indices.push_back(index + 0);
					indices.push_back(index + 1);
					indices.push_back(index + 2);
					indices.push_back(index + 0);
					indices.push_back(index + 2);
					indices.push_back(index + 3);
				}
				
				for (uint32_t i = 0; i < vertexCount; i++) {
					std::vector<std::string> indices = SplitString(tokens[i + 1], '/');
					uint32_t positionIndex = std::stoul(indices[0]) - 1;
					uint32_t uvIndex = std::stoul(indices[1]) - 1;
					uint32_t normalIndex = std::stoul(indices[2]) - 1;

					positions.emplace_back(uniquePositions[positionIndex]);
					normals.emplace_back(uniqueNormals[normalIndex]);
					uvs.emplace_back(uniqueUVs[uvIndex]);
				}

				index += vertexCount;
			}
		}

		return MeshData(positions, normals, uvs, tangents, bitangents, indices);
	}

	static void MeshProcessMoveOriginToCenter(MeshData &data) {
		Vector3 origin = Vector3(0);

		for (const Vector3 &position : data.Positions) {
			origin += position;
		}

		origin /= data.Positions.size();

		for (Vector3 &position : data.Positions) {
			position -= origin;
		}
		
		data.BoundingBox.Position = Vector3(0.0f);
	}

	static void MeshProcessGenerateMeshNormals(MeshData &data) {
		data.Normals.reserve(data.Positions.size());
		
		for (size_t i = 0; i < data.Indices.size(); i += 3) {
			Vector3 a = data.Positions[data.Indices[i + 1]] - data.Positions[data.Indices[i + 0]];
			Vector3 b = data.Positions[data.Indices[i + 2]] - data.Positions[data.Indices[i + 0]];

			Vector3 normal = Vector3::Normalize(Vector3::Cross(a, b));

			data.Normals[data.Indices[i + 0]] = normal;
			data.Normals[data.Indices[i + 1]] = normal;
			data.Normals[data.Indices[i + 2]] = normal;
		}
	}

	static void MeshProcessGenerateUVs(MeshData &data) {
		
	}

	static void MeshProcessNormalizeMesh(MeshData &data) {
		Vector3 min = Vector3(__FLT_MAX__);
		Vector3 max = Vector3(-__FLT_MAX__);

		for (const Vector3 &position : data.Positions) {
			min = Vector3::Min(min, position);
			max = Vector3::Max(max, position);
		}

		float size = Vector3::Length(max - min);

		for (Vector3 &position : data.Positions) {
			position = (position - min) / size;
		}

		data.BoundingBox.Size = max - min;
	}

	static void MeshProcessGenerateTangents(MeshData &data) {
		for (size_t i = 0; i < data.Indices.size(); i += 3) {
			uint32_t index0 = data.Indices[i + 0];
			uint32_t index1 = data.Indices[i + 1];
			uint32_t index2 = data.Indices[i + 2];

			const Vector3 edge0 = data.Positions[index1] - data.Positions[index0];
			const Vector3 edge1 = data.Positions[index2] - data.Positions[index0];
			const Vector2 deltaUV0 = data.UVs[index1] - data.UVs[index0];
			const Vector2 deltaUV1 = data.UVs[index2] - data.UVs[index0];

			Vector3 tangent, bitangent;
			float f = 1.0f / (deltaUV0.X * deltaUV1.Y - deltaUV1.X * deltaUV0.Y);

			tangent.X = f * (deltaUV1.Y * edge0.X - deltaUV0.Y * edge1.X);
			tangent.Y = f * (deltaUV1.Y * edge0.Y - deltaUV0.Y * edge1.Y);
			tangent.Z = f * (deltaUV1.Y * edge0.Z - deltaUV0.Y * edge1.Z);

			bitangent.X = f * (-deltaUV1.X * edge0.X + deltaUV0.X * edge1.X);
			bitangent.Y = f * (-deltaUV1.X * edge0.Y + deltaUV0.X * edge1.Y);
			bitangent.Z = f * (-deltaUV1.X * edge0.Z + deltaUV0.X * edge1.Z);

			tangent = Vector3::Normalize(tangent);
			bitangent = Vector3::Normalize(bitangent);

			data.Tangents[index0] = tangent;
			data.Tangents[index1] = tangent;
			data.Tangents[index2] = tangent;
			
			data.Bitangents[index0] = bitangent;
			data.Bitangents[index1] = bitangent;
			data.Bitangents[index2] = bitangent;
		}
	}

	static void MeshProcessGenerateBoundingBox(MeshData &data) {
		Vector3 min = Vector3(__FLT_MAX__);
		Vector3 max = Vector3(-__FLT_MAX__);

		for (const Vector3 &position : data.Positions) {
			min = Vector3::Min(min, position);
			max = Vector3::Max(max, position);
		}

		data.BoundingBox = AABB(max - min, min + max / 2.0f);
	}

	void ProcessMesh(MeshData &data, MeshProcess process) {
		switch (process) {
			case MeshProcess::MoveOriginToCenter:
				MeshProcessMoveOriginToCenter(data);
				break;
			case MeshProcess::GenerateNormals:
				MeshProcessGenerateMeshNormals(data);
				break;
			case MeshProcess::GenerateUVs:
				MeshProcessGenerateUVs(data);
				break;
			case MeshProcess::NormalizeMesh:
				MeshProcessNormalizeMesh(data);
				break;
			case MeshProcess::GenerateTangents:
				MeshProcessGenerateTangents(data);
				break;
			case MeshProcess::GenerateBoundingBox:
				MeshProcessGenerateBoundingBox(data);
				break;
		}
	}

}