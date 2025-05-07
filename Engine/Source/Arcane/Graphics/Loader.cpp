#include "Loader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>

#include <Arcane/Util/StringUtils.hpp>
#include <Arcane/Math/Math.hpp>

#include <Arcane/Util/ByteUtil.hpp>

#include <Arcane/File/Json.hpp>

namespace Arcane {

	enum class GltfAttributeType {
		POSITION, NORMAL, TANGENT,
		COLOR_0, COLOR_1, COLOR_2,
		TEXCOORD_0, TEXCOORD_1, TEXCOORD_2
	};

	enum class GltfAccessorElementType {
		SCALAR, VEC2, VEC3, VEC4, MAT2, MAT3, MAT4
	};

	enum class GltfComponentType {
		BYTE, UNSIGNED_BYTE, SHORT, UNSIGNED_SHORT, UNSIGNED_INT, FLOAT
	};

	enum class GltfBufferTarget {
		ARRAY_BUFFER, ELEMENT_ARRAY_BUFFER
	};

	GltfComponentType GetComponentType(const uint32_t type) {
		switch (type) {
			case 5120: return GltfComponentType::BYTE;
			case 5121: return GltfComponentType::UNSIGNED_BYTE;
			case 5122: return GltfComponentType::SHORT;
			case 5123: return GltfComponentType::UNSIGNED_SHORT;
			case 5125: return GltfComponentType::UNSIGNED_INT;
			case 5126: return GltfComponentType::FLOAT;
			default: return ((GltfComponentType)UINT32_MAX);
		}
	}

	GltfAccessorElementType GetAccessorElementType(const std::string &element) {
		if (element == "SCALAR") return GltfAccessorElementType::SCALAR;
		else if (element == "VEC2") return GltfAccessorElementType::VEC2;
		else if (element == "VEC3") return GltfAccessorElementType::VEC3;
		else if (element == "VEC4") return GltfAccessorElementType::VEC4;
		else if (element == "MAT2") return GltfAccessorElementType::MAT2;
		else if (element == "MAT3") return GltfAccessorElementType::MAT3;
		else if (element == "MAT4") return GltfAccessorElementType::MAT4;
		return ((GltfAccessorElementType)UINT32_MAX);
	}

	GltfBufferTarget GetBufferTarget(uint32_t target) {
		switch (target) {
			case 34962: return GltfBufferTarget::ARRAY_BUFFER;
			case 34963: return GltfBufferTarget::ELEMENT_ARRAY_BUFFER;
			default: return ((GltfBufferTarget)UINT32_MAX);
		}
	}

	size_t GetComponentSize(GltfComponentType component) {
		switch (component) {
			case GltfComponentType::BYTE: return sizeof(int8_t);
			case GltfComponentType::UNSIGNED_BYTE: return sizeof(uint8_t);
			case GltfComponentType::SHORT: return sizeof(int16_t);
			case GltfComponentType::UNSIGNED_SHORT: return sizeof(uint16_t);
			case GltfComponentType::UNSIGNED_INT: return sizeof(uint32_t);
			case GltfComponentType::FLOAT: return sizeof(float);
			default: return SIZE_MAX;
		}
	}

	uint32_t GetElementTypeCount(GltfAccessorElementType element) {
		switch (element) {
			case GltfAccessorElementType::SCALAR: return 1;
			case GltfAccessorElementType::VEC2: return 2;
			case GltfAccessorElementType::VEC3: return 3;
			case GltfAccessorElementType::VEC4: return 4;
			case GltfAccessorElementType::MAT2: return 4;
			case GltfAccessorElementType::MAT3: return 9;
			case GltfAccessorElementType::MAT4: return 16;
			default: return UINT32_MAX;
		}
	}

	struct GltfAttribute {
		GltfAttributeType Type;
		uint32_t AccessorIndex;
	};

	struct GltfPrimitiveDesc {
		GltfAttribute Attributes[8];
		uint32_t AttributeCount;
		uint32_t IndexBufferAccessor;
		uint32_t MaterialAccessor;
		uint32_t ModeIndex;
	};

	struct GltfMeshDesc {
		GltfPrimitiveDesc Primitives[8];
		uint32_t PrimitiveCount;
	};

	struct GltfNodeDesc {
		uint32_t MeshIndex;
		uint32_t MaterialIndex;
	};

	struct GltfAccessorDesc {
		uint32_t BufferViewIndex;
		uint32_t ByteOffset;
		GltfComponentType ComponentType;
		bool Normalized;
		uint32_t Count;
		GltfAccessorElementType Type;
	};

	struct GltfBufferViewDesc {
		uint32_t BufferIndex;
		uint32_t ByteOffset;
		uint32_t ByteLength;
		uint32_t ByteStride;
		GltfBufferTarget Target;
	};

	struct GltfBufferDesc {
		std::string URI;
		uint32_t ByteLength;
	};

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

		const std::vector<Vector3> tangents = {
			{ 1, 0, 0 },
			{ 1, 0, 0 },
			{ 1, 0, 0 },
			{ 1, 0, 0 },

			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 0, 0, 1 },

			{ -1, 0, 0 },
			{ -1, 0, 0 },
			{ -1, 0, 0 },
			{ -1, 0, 0 },

			{ 0, 0, -1 },
			{ 0, 0, -1 },
			{ 0, 0, -1 },
			{ 0, 0, -1 },

			{ 1, 0, 0 },
			{ 1, 0, 0 },
			{ 1, 0, 0 },
			{ 1, 0, 0 },

			{ -1, 0, 0 },
			{ -1, 0, 0 },
			{ -1, 0, 0 },
			{ -1, 0, 0 },
		};

		std::vector<Vector3> bitangents = {
			{ 0, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 1, 0 },

			{ 0, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 1, 0 },

			{ 0, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 1, 0 },

			{ 0, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 1, 0 },
			{ 0, 1, 0 },

			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 0, 0, 1 },

			{ 0, 0, -1 },
			{ 0, 0, -1 },
			{ 0, 0, -1 },
			{ 0, 0, -1 },
		};

		return MeshData(positions, normals, uvs, tangents, bitangents, indices);
	}

	MeshData LoadMesh(const std::string &path) {
		FILE *f = fopen(path.c_str(), "rb");
		AR_ASSERT(f, "Could not open file: %s\n", path.c_str());

		uint32_t header[3];
		fread(header, 4, 3, f);
		AR_ASSERT(header[0] == 0x46546C67, "%s is not a glTF file\n", path.c_str());

		uint32_t chunkHeader[2];
		void *chunkData;

		GltfNodeDesc *nodeDescs;
		uint32_t nodeCount;

		GltfMeshDesc *meshDescs;
		uint32_t meshCount;

		GltfAccessorDesc *accessorDescs;
		uint32_t accessorCount;

		GltfBufferViewDesc *bufferViewDescs;
		uint32_t bufferViewCount;

		GltfBufferDesc *bufferDescs;
		uint32_t bufferCount;

		uint8_t *binaryData;

		fread(chunkHeader, 4, 2, f);
		if (chunkHeader[1] == 0x4E4F534A) {
			chunkData = malloc(chunkHeader[0] + 1);
			memset(chunkData, 0, chunkHeader[0] + 1);
			fread(chunkData, 1, chunkHeader[0], f);

			JsonDocument doc = ParseJson((char*)chunkData);

			const JsonValue &allNodes = doc["nodes"];
			const JsonValue &allMeshes = doc["meshes"];
			const JsonValue &allAccessors = doc["accessors"];
			const JsonValue &allBufferViews = doc["bufferViews"];
			const JsonValue &allBuffers = doc["buffers"];

			uint32_t sceneIndex = doc["scene"].GetUint();
			const JsonValue &scene = doc["scenes"][sceneIndex];

			nodeCount = allNodes.Size();
			nodeDescs = new GltfNodeDesc[nodeCount];
			std::memset(nodeDescs, 0, nodeCount* sizeof(GltfNodeDesc));

			for (uint32_t i = 0; i < nodeCount; i++) {
				const JsonValue &node = allNodes[i];
				GltfNodeDesc &nodeDesc = nodeDescs[i];

				nodeDesc.MeshIndex = node.HasMember("mesh") ? node["mesh"].GetUint() : UINT32_MAX;
				nodeDesc.MaterialIndex = node.HasMember("material") ? node["material"].GetUint() : UINT32_MAX;
			}

			meshCount = allMeshes.Size();
			meshDescs = new GltfMeshDesc[meshCount];
			std::memset(meshDescs, 0, meshCount * sizeof(GltfMeshDesc));

			for (uint32_t i = 0; i < meshCount; i++) {
				const JsonValue &mesh = allMeshes[i];
				GltfMeshDesc &meshDesc = meshDescs[i];

				const JsonValue &primitives = mesh["primitives"];
				meshDesc.PrimitiveCount = primitives.Size();

				for (uint32_t j = 0; j < meshDesc.PrimitiveCount; j++) {
					const JsonValue &primitive = primitives[j];
					GltfPrimitiveDesc &primitiveDesc = meshDesc.Primitives[j]; 
					
					const JsonValue &attributes = primitive["attributes"];

					if (attributes.HasMember("POSITION")) {
						uint32_t attributeIndex = primitiveDesc.AttributeCount++;
						primitiveDesc.Attributes[attributeIndex].Type = GltfAttributeType::POSITION;
						primitiveDesc.Attributes[attributeIndex].AccessorIndex = attributes["POSITION"].GetUint();
					}
					if (attributes.HasMember("NORMAL")) {
						uint32_t attributeIndex = primitiveDesc.AttributeCount++;
						primitiveDesc.Attributes[attributeIndex].Type = GltfAttributeType::NORMAL;
						primitiveDesc.Attributes[attributeIndex].AccessorIndex = attributes["NORMAL"].GetUint();
					}
					if (attributes.HasMember("TEXCOORD_0")) {
						uint32_t attributeIndex = primitiveDesc.AttributeCount++;
						primitiveDesc.Attributes[attributeIndex].Type = GltfAttributeType::TEXCOORD_0;
						primitiveDesc.Attributes[attributeIndex].AccessorIndex = attributes["TEXCOORD_0"].GetUint();
					}
					if (attributes.HasMember("TANGENT")) {
						uint32_t attributeIndex = primitiveDesc.AttributeCount++;
						primitiveDesc.Attributes[attributeIndex].Type = GltfAttributeType::TANGENT;
						primitiveDesc.Attributes[attributeIndex].AccessorIndex = attributes["TANGENT"].GetUint();
					}
					if (attributes.HasMember("COLOR_0")) {
						uint32_t attributeIndex = primitiveDesc.AttributeCount++;
						primitiveDesc.Attributes[attributeIndex].Type = GltfAttributeType::COLOR_0;
						primitiveDesc.Attributes[attributeIndex].AccessorIndex = attributes["COLOR_0"].GetUint();
					}
					primitiveDesc.IndexBufferAccessor = primitive["indices"].GetUint();
					primitiveDesc.MaterialAccessor = primitive.HasMember("material") ? primitive["material"].GetUint() : UINT32_MAX;
					primitiveDesc.ModeIndex = primitive.HasMember("mode") ? primitive["mode"].GetUint() : UINT32_MAX;
				}

			}

			accessorCount = allAccessors.Size();
			accessorDescs = new GltfAccessorDesc[accessorCount];
			std::memset(accessorDescs, 0, accessorCount * sizeof(GltfAccessorDesc));

			for (uint32_t i = 0; i < accessorCount; i++) {
				const JsonValue &accessor = allAccessors[i];
				GltfAccessorDesc &accessorDesc = accessorDescs[i];

				accessorDesc.BufferViewIndex = accessor.HasMember("bufferView") ? accessor["bufferView"].GetUint() : UINT32_MAX;
				accessorDesc.ByteOffset = accessor.HasMember("byteOffset") ? accessor["byteOffset"].GetUint() : 0;
				accessorDesc.ComponentType = GetGltfComponentType(accessor["componentType"].GetUint());
				accessorDesc.Normalized = accessor.HasMember("normalized") ? accessor["normalized"].GetBool() : false;
				accessorDesc.Count = accessor["count"].GetUint();
				accessorDesc.Type = GetAccessorElementType(accessor["type"].GetString());
			}

			bufferViewCount = allBufferViews.Size();
			bufferViewDescs = new GltfBufferViewDesc[bufferViewCount];
			std::memset(bufferViewDescs, 0, bufferViewCount * sizeof(GltfBufferViewDesc));

			for (uint32_t i = 0; i < bufferViewCount; i++) {
				const JsonValue &bufferView = allBufferViews[i];
				GltfBufferViewDesc &bufferViewDesc = bufferViewDescs[i];

				bufferViewDesc.BufferIndex = bufferView["buffer"].GetUint();
				bufferViewDesc.ByteOffset = bufferView.HasMember("byteOffset") ? bufferView["byteOffset"].GetUint() : 0;
				bufferViewDesc.ByteLength = bufferView["byteLength"].GetUint();
				bufferViewDesc.ByteStride = bufferView.HasMember("byteStride") ? bufferView["byteStride"].GetUint() : UINT32_MAX;
				bufferViewDesc.Target = bufferView.HasMember("target") ? GetBufferTarget(bufferView["target"].GetUint()) : ((GltfBufferTarget)UINT32_MAX);
			}

			bufferCount = allBuffers.Size();
			bufferDescs = new GltfBufferDesc[bufferCount];
			std::memset(bufferDescs, 0, bufferCount * sizeof(GltfBufferDesc));

			for (uint32_t i = 0; i < bufferCount; i++) {
				const JsonValue &buffer = allBuffers[i];
				GltfBufferDesc &bufferDesc = bufferDescs[i];

				bufferDesc.URI = buffer.HasMember("uri") ? buffer["uri"].GetString() : "";
				bufferDesc.ByteLength = buffer["byteLength"].GetUint();
			}

			free(chunkData);
		}

		fread(chunkHeader, 4, 2, f);
		binaryData = (uint8_t*)malloc(chunkHeader[0]);
		fread(binaryData, 1, chunkHeader[0], f);

		fclose(f);

		MeshData data;

		return data;
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
		}
	}

}