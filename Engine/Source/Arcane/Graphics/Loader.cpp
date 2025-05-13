#include "Loader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>

#include <Arcane/Math/Math.hpp>
#include <Arcane/Math/Vector4.hpp>
#include <Arcane/Math/Quaternion.hpp>
#include <Arcane/Math/Matrix4.hpp>
#include <Arcane/Util/StringUtils.hpp>
#include <Arcane/Util/ByteUtil.hpp>
#include <Arcane/File/Json.hpp>
#include <Arcane/Util/BufferView.hpp>
#include <Arcane/System/Time.hpp>

#include "Transform.hpp"

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
		Matrix4 Transform;
		Matrix4 TransformNoScale;
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
		AR_PROFILE_FUNCTION();
		
		FILE *f = fopen(path.c_str(), "rb");
		AR_ASSERT(f, "Could not open file: %s\n", path.c_str());

		const uint64_t startTime = GetCurrentTimeMillis();

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

			for (uint32_t i = 0; i < nodeCount; i++) {
				const JsonValue &node = allNodes[i];
				GltfNodeDesc &nodeDesc = nodeDescs[i];

				nodeDesc.MeshIndex = node.HasMember("mesh") ? node["mesh"].GetUint() : UINT32_MAX;
				nodeDesc.MaterialIndex = node.HasMember("material") ? node["material"].GetUint() : UINT32_MAX;

				Transform transform;

				if (node.HasMember("translation")) {
					transform.Position.X = node["translation"][0].GetFloat();
					transform.Position.Y = node["translation"][1].GetFloat();
					transform.Position.Z = node["translation"][2].GetFloat();
				}

				if (node.HasMember("rotation")) {
					Quaternion q;
					q.X = node["rotation"][0].GetFloat();
					q.Y = node["rotation"][1].GetFloat();
					q.Z = node["rotation"][2].GetFloat();
					q.W = ToDegrees(node["rotation"][3].GetFloat());
				
					transform.Rotation = q.ToEuler();
					transform.Rotation.X = transform.Rotation.X;
					transform.Rotation.Y = transform.Rotation.Y;
					transform.Rotation.Z = transform.Rotation.Z;
				}
				nodeDesc.TransformNoScale = transform.GetModelMatrix();

				if (node.HasMember("scale")) {
					transform.Scale.X = node["scale"][0].GetFloat();
					transform.Scale.Y = node["scale"][1].GetFloat();
					transform.Scale.Z = node["scale"][2].GetFloat();
				}

				nodeDesc.Transform = transform.GetModelMatrix();
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
				accessorDesc.ComponentType = GetComponentType(accessor["componentType"].GetUint());
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

		const uint32_t meshIndex = nodeDescs[0].MeshIndex;
		const GltfPrimitiveDesc &primitive = meshDescs[meshIndex].Primitives[0];

		for (uint32_t i = 0; i < primitive.AttributeCount; i++) {
			const GltfAttribute &attribute = primitive.Attributes[i];
			const GltfAccessorDesc &accessor = accessorDescs[attribute.AccessorIndex];
			const GltfBufferViewDesc &bufferView = bufferViewDescs[accessor.BufferViewIndex];
			const GltfBufferDesc &buffer = bufferDescs[bufferView.BufferIndex];

			BufferView view(binaryData, bufferView.ByteOffset, bufferView.ByteLength);

			if (attribute.Type == GltfAttributeType::POSITION) {
				data.Positions.resize(accessor.Count);
				if (accessor.ComponentType == GltfComponentType::FLOAT) {
					for (uint32_t j = 0; j < accessor.Count; j++) {
						Vector4 transformed = nodeDescs[0].Transform * Vector4(
							-ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
							ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
							ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
							1.0
						);

						data.Positions[j] = { transformed.X, transformed.Y, transformed.Z };
					}
				}
			} else if (attribute.Type == GltfAttributeType::NORMAL) {
				data.Normals.resize(accessor.Count);
				data.Tangents.resize(accessor.Count);
				data.Bitangents.resize(accessor.Count);
				if (accessor.ComponentType == GltfComponentType::FLOAT) {
					for (uint32_t j = 0; j < accessor.Count; j++) {
						Vector4 transformed = nodeDescs[0].TransformNoScale * Vector4(
							-ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
							ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
							ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
							1.0
						);

						Vector3 &v = data.Normals[j] = { transformed.X, transformed.Y, transformed.Z };
						v = Vector3::Normalize(v);
					}
				}
			} else if (attribute.Type == GltfAttributeType::TEXCOORD_0) {
				data.UVs.resize(accessor.Count);
				if (accessor.ComponentType == GltfComponentType::FLOAT) {
					for (uint32_t j = 0; j < accessor.Count; j++) {
						data.UVs[j] = {
							ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
							ToNativeEndian<Endianness::LittleEndian>(view.Next<float>())
						};
					}
				} else if (accessor.ComponentType == GltfComponentType::UNSIGNED_BYTE) {
					for (uint32_t j = 0; j < accessor.Count; j++) {
						data.UVs[j] = {
							ToNativeEndian<Endianness::LittleEndian>(view.Next<uint8_t>()),
							ToNativeEndian<Endianness::LittleEndian>(view.Next<uint8_t>())
						};
					}
				} else if (accessor.ComponentType == GltfComponentType::UNSIGNED_SHORT) {
					for (uint32_t j = 0; j < accessor.Count; j++) {
						data.UVs[j] = {
							ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>()),
							ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>())
						};
					}
				}
			}
		}

		if (primitive.IndexBufferAccessor != UINT32_MAX) {
			const GltfAccessorDesc &accessor = accessorDescs[primitive.IndexBufferAccessor];
			const GltfBufferViewDesc &bufferView = bufferViewDescs[accessor.BufferViewIndex];
			const GltfBufferDesc &buffer = bufferDescs[bufferView.BufferIndex];

			BufferView view(binaryData, bufferView.ByteOffset, bufferView.ByteLength);

			data.Indices.reserve(accessor.Count);
			if (accessor.ComponentType == GltfComponentType::UNSIGNED_INT) {
				for (uint32_t i = 0; i < accessor.Count; i++) {
					data.Indices.emplace_back(
						ToNativeEndian<Endianness::LittleEndian>(view.Next<uint32_t>())
					);
				}
			} else if (accessor.ComponentType == GltfComponentType::UNSIGNED_SHORT) {
				for (uint32_t i = 0; i < accessor.Count; i++) {
					data.Indices.emplace_back(
						ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>())
					);
				}
			}
		}

		printf("Loading: %s (%llums)\n", path.c_str(), GetCurrentTimeMillis() - startTime);
		printf("\t%u vertices\n", data.Positions.size());
		printf("\t%u indices\n", data.Indices.size());

		return data;
	}

}