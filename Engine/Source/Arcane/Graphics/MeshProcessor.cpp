#include "MeshProcessor.hpp"

#include <Arcane/System/Time.hpp>

namespace Arcane {

	static void MeshProcess_MoveOriginToCenter(MeshData &data) {
		AR_PROFILE_FUNCTION();
		
		Vector3 origin = Vector3(0);

		for (const Vector3 &position : data.Positions) {
			origin += position;
		}

		origin /= data.Positions.size();

		for (Vector3 &position : data.Positions) {
			position -= origin;
		}
	}

	static void TriangleProcess_GenerateNormals(MeshData &data, const Triangle &t) {
		AR_PROFILE_FUNCTION();
		Vector3 a = data.Positions[data.Indices[t.Indices[1]]] - data.Positions[data.Indices[t.Indices[0]]];
		Vector3 b = data.Positions[data.Indices[t.Indices[2]]] - data.Positions[data.Indices[t.Indices[0]]];

		Vector3 normal = Vector3::Normalize(Vector3::Cross(a, b));

		data.Normals[data.Indices[t.Indices[0]]] = normal;
		data.Normals[data.Indices[t.Indices[1]]] = normal;
		data.Normals[data.Indices[t.Indices[2]]] = normal;
	}

	static void MeshProcess_GenerateUVs(MeshData &data) {
		AR_PROFILE_FUNCTION();
		
		data.UVs.resize(data.Positions.size());

		for (size_t i = 0; i < data.Positions.size(); ++i) {
			const Vector3 &position = data.Positions[i];

			// Project the texture onto the front and back (Z-axis projection)
			// Map X and Y to UV coordinates, assuming the texture spans from 0 to 1
			Vector2 uv = Vector2(
				(position.X + 1.0f) * 0.5f, // Normalize X to [0, 1]
				(position.Y + 1.0f) * 0.5f  // Normalize Y to [0, 1]
			);

			data.UVs[i] = uv;
		}
	}

	static void MeshProcess_Normalize(MeshData &data) {
		AR_PROFILE_FUNCTION();
		
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
	}

	static void TriangleProcess_GenerateTangents(MeshData &data, const Triangle &t) {
		AR_PROFILE_FUNCTION();

		uint32_t index0 = data.Indices[t.Indices[0]];
		uint32_t index1 = data.Indices[t.Indices[1]];
		uint32_t index2 = data.Indices[t.Indices[2]];

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

		bitangent = -bitangent;

		data.Tangents[index0] = tangent;
		data.Tangents[index1] = tangent;
		data.Tangents[index2] = tangent;
		
		data.Bitangents[index0] = bitangent;
		data.Bitangents[index1] = bitangent;
		data.Bitangents[index2] = bitangent;
	}

	static void TriangleProcess_SwapWindingOrder(MeshData &data, const Triangle &t) {
		uint32_t index0 = data.Indices[t.Indices[0]];
		uint32_t index1 = data.Indices[t.Indices[1]];
		uint32_t index2 = data.Indices[t.Indices[2]];

		data.Indices[t.Indices[0]] = index2;
		data.Indices[t.Indices[2]] = index0;
	}

	static void MeshProcess_GenerateBoundingBox(MeshData &data) {
		AR_PROFILE_FUNCTION();
		
		Vector3 min = Vector3(__FLT_MAX__);
		Vector3 max = Vector3(-__FLT_MAX__);

		for (const Vector3 &position : data.Positions) {
			min = Vector3::Min(min, position);
			max = Vector3::Max(max, position);
		}
	}

	MeshProcessor::MeshProcessor() { }

	MeshProcessor::MeshProcessor(const std::initializer_list<MeshProcess> &processes) : mProcesses(processes) { }

	MeshProcessor::~MeshProcessor() { }

	MeshData &MeshProcessor::Process(MeshData &data) const {
		uint64_t processBegin = GetCurrentTimeMillis();

		for (uint32_t i = 0; i < data.Indices.size(); i += 3) {
			Triangle t;
			t.Indices[0] = i;
			t.Indices[1] = i + 1;
			t.Indices[2] = i + 2;

			for (const MeshProcess &process : mProcesses) {
				switch (process) {
					case MeshProcess::GenerateNormals: TriangleProcess_GenerateNormals(data, t); break;
					case MeshProcess::GenerateTangents: TriangleProcess_GenerateTangents(data, t); break;
					case MeshProcess::SwapWindingOrder: TriangleProcess_SwapWindingOrder(data, t); break;
				}
			}
		}

		printf("Mesh Processing Took: %llums\n", GetCurrentTimeMillis() - processBegin);

		return data;
	}

}