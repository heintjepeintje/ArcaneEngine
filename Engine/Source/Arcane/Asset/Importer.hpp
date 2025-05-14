#pragma once

#include <Arcane/Math/Vector4.hpp>
#include <Arcane/Math/Vector3.hpp>
#include <Arcane/Math/Vector2.hpp>
#include <Arcane/Physics/AABB.hpp>
#include <filesystem>

namespace Arcane {

	enum ImportFlags {
		ImportFlag_None = 0,
		ImportFlag_GenerateNormals = AR_BIT(0),
		ImportFlag_GenerateTangents = AR_BIT(1),
		ImportFlag_FlipUVs = AR_BIT(2),
		ImportFlag_SwapWindingOrder = AR_BIT(3),
		ImportFlag_GenerateBoundingBox = AR_BIT(4)
	};

	struct MeshData {
		char *Name;

		bool HasPositions;
		Vector3 *Positions;
		bool HasNormals;
		Vector3 *Normals;
		bool HasUVs;
		Vector2 *UVs;
		bool HasTangents;
		Vector3 *Tangents;
		Vector3 *Bitangents;
		uint32_t VertexCount;

		bool HasIndices;
		uint32_t *Indices;
		uint32_t IndexCount;
	};

	struct MaterialData {
		char *Name;
		char *AlbedoMap;
		char *NormalMap;
		char *MetallicMap;
		char *RoughnessMap;
		char *AmbientOcclusionMap;
	};

	struct Node {
		MeshData Mesh;
		MaterialData Material;
		AABB BoundingBox;
	};

	class Importer {
	public:
		static Ref<Importer> Create(const std::filesystem::path &path);

	public:
		Importer() = default;
		virtual ~Importer() = default;

		virtual bool Import(uint32_t flags) = 0;

		virtual const Node &GetNode(uint32_t index) const = 0;
		virtual uint32_t GetNodeCount() const = 0;
	};

}