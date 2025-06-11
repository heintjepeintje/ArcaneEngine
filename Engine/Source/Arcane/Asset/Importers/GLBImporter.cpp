#include "GLBImporter.hpp"

#include <Arcane/Math/Matrix4.hpp>
#include <Arcane/Math/Quaternion.hpp>
#include <Arcane/Util/StringUtils.hpp>
#include <Arcane/Util/ByteUtil.hpp>
#include <Arcane/File/Json.hpp>
#include <Arcane/Util/BufferView.hpp>
#include <Arcane/Graphics/Transform.hpp>

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
		Matrix4 NormalTransform;
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

	bool ImportGLB(const std::filesystem::path &path, uint32_t flags, std::vector<Node> &outNodes) {
		FILE *f = fopen(path.string().c_str(), "rb");
		if (!f) {
			AR_ASSERT(false, "Could not open file: %s\n", path.string().c_str());
			return false;
		}

		uint32_t header[3];
		fread(header, 4, 3, f);
		if (header[0] != 0x46546C67) {
			AR_ASSERT(false, "%s is not a glTF file\n", path.string().c_str());
			return false;
		}

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

				Transform transform, normalTransform;

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
					normalTransform.Rotation = q.ToEuler();
				}

				if (node.HasMember("scale")) {
					transform.Scale.X = node["scale"][0].GetFloat();
					transform.Scale.Y = node["scale"][1].GetFloat();
					transform.Scale.Z = node["scale"][2].GetFloat();
				}

				nodeDesc.Transform = transform.GetModelMatrix();
				nodeDesc.NormalTransform = normalTransform.GetModelMatrix();
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

		outNodes.resize(nodeCount);

		for (uint32_t nodeIndex = 0; nodeIndex < nodeCount; nodeIndex++) {
			Node &node = outNodes[nodeIndex];
			
			if (flags & ImportFlag_GenerateBoundingBox) {
				node.BoundingBox.Min = Vector3::MaxValue();
				node.BoundingBox.Max = Vector3::MinValue();
			}

			const uint32_t meshIndex = nodeDescs[nodeIndex].MeshIndex;
			if (meshIndex != UINT32_MAX) {
				const GltfPrimitiveDesc &primitive = meshDescs[meshIndex].Primitives[0];

				for (uint32_t attrIndex = 0; attrIndex < primitive.AttributeCount; attrIndex++) {
					const GltfAttribute &attribute = primitive.Attributes[attrIndex];
					const GltfAccessorDesc &accessor = accessorDescs[attribute.AccessorIndex];
					const GltfBufferViewDesc &bufferView = bufferViewDescs[accessor.BufferViewIndex];
					const GltfBufferDesc &buffer = bufferDescs[bufferView.BufferIndex];

					BufferView view(binaryData, bufferView.ByteOffset, bufferView.ByteLength);

					if (attribute.Type == GltfAttributeType::POSITION) {
						node.Mesh.HasPositions = true;
						node.Mesh.Positions.resize(accessor.Count);
						node.Mesh.VertexCount = accessor.Count;

						if (accessor.ComponentType == GltfComponentType::FLOAT) {
							for (uint32_t componentIndex = 0; componentIndex < accessor.Count; componentIndex++) {
								Vector4 transformed = nodeDescs[nodeIndex].Transform * Vector4(
									-ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
									ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
									ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
									1.0
								);

								Vector3 position = { transformed.X, transformed.Y, transformed.Z };

								if (flags & ImportFlag_GenerateBoundingBox) {
									node.BoundingBox.Min = Vector3::Min(node.BoundingBox.Min, position);
									node.BoundingBox.Max = Vector3::Max(node.BoundingBox.Max, position);
								}

								node.Mesh.Positions[componentIndex] = position;
							}
						}
					} else if (attribute.Type == GltfAttributeType::NORMAL) {
						node.Mesh.HasNormals = true;
						node.Mesh.Normals.resize(accessor.Count);

						if (accessor.ComponentType == GltfComponentType::FLOAT) {
							for (uint32_t componentIndex = 0; componentIndex < accessor.Count; componentIndex++) {
								Vector4 transformed = nodeDescs[nodeIndex].NormalTransform * Vector4(
									-ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
									ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
									ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
									1.0
								);

								node.Mesh.Normals[componentIndex] = Vector3::Normalize({transformed.X, transformed.Y, transformed.Z });
							}
						}
					} else if (attribute.Type == GltfAttributeType::TANGENT) {
						node.Mesh.HasTangents = true;
						node.Mesh.Tangents.resize(accessor.Count);
						node.Mesh.Bitangents.resize(accessor.Count);
					} else if (attribute.Type == GltfAttributeType::TEXCOORD_0) {
						node.Mesh.HasUVs = true;
						node.Mesh.UVs.resize(accessor.Count);

						if (accessor.ComponentType == GltfComponentType::FLOAT) {
							for (uint32_t componentIndex = 0; componentIndex < accessor.Count; componentIndex++) {
								if (flags & ImportFlag_FlipUVs) {
									node.Mesh.UVs[componentIndex] = {
										ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
										1.0f - ToNativeEndian<Endianness::LittleEndian>(view.Next<float>())
									};
								} else {
									node.Mesh.UVs[componentIndex] = {
										ToNativeEndian<Endianness::LittleEndian>(view.Next<float>()),
										ToNativeEndian<Endianness::LittleEndian>(view.Next<float>())
									};
								}
							}
						} else if (accessor.ComponentType == GltfComponentType::UNSIGNED_BYTE) {
							for (uint32_t componentIndex = 0; componentIndex < accessor.Count; componentIndex++) {
								if (flags & ImportFlag_FlipUVs) {
									node.Mesh.UVs[componentIndex] = {
										(float)ToNativeEndian<Endianness::LittleEndian>(view.Next<uint8_t>()),
										1.0f - (float)ToNativeEndian<Endianness::LittleEndian>(view.Next<uint8_t>())
									};
								} else {
									node.Mesh.UVs[componentIndex] = {
										(float)ToNativeEndian<Endianness::LittleEndian>(view.Next<uint8_t>()),
										(float)ToNativeEndian<Endianness::LittleEndian>(view.Next<uint8_t>())
									};
								}
							}
						} else if (accessor.ComponentType == GltfComponentType::UNSIGNED_SHORT) {
							for (uint32_t componentIndex = 0; componentIndex < accessor.Count; componentIndex++) {
								if (flags & ImportFlag_FlipUVs) {
									node.Mesh.UVs[componentIndex] = {
										(float)ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>()),
										1.0f - (float)ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>())
									};
								} else {
									node.Mesh.UVs[componentIndex] = {
										(float)ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>()),
										(float)ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>())
									};
								}
							}
						}
					}
				}

				if (primitive.IndexBufferAccessor != UINT32_MAX) {
					const GltfAccessorDesc &accessor = accessorDescs[primitive.IndexBufferAccessor];
					const GltfBufferViewDesc &bufferView = bufferViewDescs[accessor.BufferViewIndex];
					const GltfBufferDesc &buffer = bufferDescs[bufferView.BufferIndex];

					BufferView view(binaryData, bufferView.ByteOffset, bufferView.ByteLength);

					node.Mesh.HasIndices = true;
					node.Mesh.Indices.resize(accessor.Count);
					node.Mesh.IndexCount = accessor.Count;

					if (accessor.ComponentType == GltfComponentType::UNSIGNED_INT) {
						for (uint32_t i = 0; i < accessor.Count; i += 3) {
							if (flags & ImportFlag_SwapWindingOrder) {
								node.Mesh.Indices[i + 2] = ToNativeEndian<Endianness::LittleEndian>(view.Next<uint32_t>());
								node.Mesh.Indices[i + 1] = ToNativeEndian<Endianness::LittleEndian>(view.Next<uint32_t>());
								node.Mesh.Indices[i] = ToNativeEndian<Endianness::LittleEndian>(view.Next<uint32_t>());
							} else {
								node.Mesh.Indices[i] = ToNativeEndian<Endianness::LittleEndian>(view.Next<uint32_t>());
								node.Mesh.Indices[i + 1] = ToNativeEndian<Endianness::LittleEndian>(view.Next<uint32_t>());
								node.Mesh.Indices[i + 3] = ToNativeEndian<Endianness::LittleEndian>(view.Next<uint32_t>());
							}
						}
					} else if (accessor.ComponentType == GltfComponentType::UNSIGNED_SHORT) {
						for (uint32_t i = 0; i < accessor.Count; i += 3) {
							if (flags & ImportFlag_SwapWindingOrder) {
								node.Mesh.Indices[i] = ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>());
								node.Mesh.Indices[i + 1] = ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>());
								node.Mesh.Indices[i + 2] = ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>());
							} else {
								node.Mesh.Indices[i + 2] = ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>());
								node.Mesh.Indices[i + 1] = ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>());
								node.Mesh.Indices[i] = ToNativeEndian<Endianness::LittleEndian>(view.Next<uint16_t>());
							}
						}
					}
				}
			}
		}

		for (uint32_t nodeIndex = 0; nodeIndex < outNodes.size(); nodeIndex++) {
			Node &n = outNodes[nodeIndex];
			
			n.Mesh.Normals.resize(n.Mesh.VertexCount);
			n.Mesh.Tangents.resize(n.Mesh.VertexCount);
			n.Mesh.Bitangents.resize(n.Mesh.VertexCount);

			if (flags & ImportFlag_GenerateNormals) {
				n.Mesh.HasNormals = true;
			}

			if (flags & ImportFlag_GenerateTangents) {
				n.Mesh.HasTangents = true;
			}

			for (uint32_t i = 0; i < n.Mesh.IndexCount; i += 3) {
				uint32_t index0 = n.Mesh.Indices[i];
				uint32_t index1 = n.Mesh.Indices[i + 1];
				uint32_t index2 = n.Mesh.Indices[i + 2];

				Vector3 edge0 = n.Mesh.Positions[index1] - n.Mesh.Positions[index0];
				Vector3 edge1 = n.Mesh.Positions[index2] - n.Mesh.Positions[index0];
				
				if (!n.Mesh.HasNormals && flags & ImportFlag_GenerateNormals) {
					Vector3 normal = Vector3::Normalize(Vector3::Cross(edge0, edge1));

					n.Mesh.Normals[index0] = normal;
					n.Mesh.Normals[index1] = normal;
					n.Mesh.Normals[index2] = normal;
				}

				if (!n.Mesh.HasTangents && flags & ImportFlag_GenerateTangents) {
					Vector2 deltaUV0 = n.Mesh.UVs[index1] - n.Mesh.UVs[index0];
					Vector2 deltaUV1 = n.Mesh.UVs[index2] - n.Mesh.UVs[index0];

					float f = 1.0f / (deltaUV0.X * deltaUV1.Y - deltaUV0.Y * deltaUV1.X);
					
					Vector3 tangent = Vector3::Normalize((edge0 * deltaUV1.Y - edge1 * deltaUV0.Y) * f);
					Vector3 bitangent = -Vector3::Normalize((edge1 * deltaUV0.X + edge0 * -deltaUV1.X) * f);

					n.Mesh.Tangents[index0] = tangent;
					n.Mesh.Tangents[index1] = tangent;
					n.Mesh.Tangents[index2] = tangent;

					n.Mesh.Bitangents[index0] = bitangent;
					n.Mesh.Bitangents[index1] = bitangent;
					n.Mesh.Bitangents[index2] = bitangent;
				}
			}
		}

		delete[] nodeDescs;
		delete[] meshDescs;
		delete[] accessorDescs;
		delete[] bufferViewDescs;
		delete[] bufferDescs;

		free(binaryData);

		return true;
	}

}