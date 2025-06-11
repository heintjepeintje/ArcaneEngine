#pragma once

#include <Arcane/Math/Vector4.hpp>
#include <Arcane/Math/Vector3.hpp>
#include <Arcane/Math/Vector2.hpp>
#include <Arcane/Physics/AABB.hpp>
#include <Arcane/Graphics/MeshData.hpp>
#include <filesystem>
#include <vector>

namespace Arcane {

	enum ImportFlags {
		ImportFlag_None = 0,
		ImportFlag_GenerateNormals = AR_BIT(0),
		ImportFlag_GenerateTangents = AR_BIT(1),
		ImportFlag_FlipUVs = AR_BIT(2),
		ImportFlag_SwapWindingOrder = AR_BIT(3),
		ImportFlag_GenerateBoundingBox = AR_BIT(4)
	};

	struct MaterialData {
		std::string Name;
		std::string AlbedoMap;
		std::string NormalMap;
		std::string MetallicMap;
		std::string RoughnessMap;
		std::string AmbientOcclusionMap;
	};

	struct Node {
		std::string Name;
		MeshData Mesh;
		MaterialData Material;
		AABB BoundingBox;
	};

	class Importer {
	public:
		Importer();
		virtual ~Importer();

		bool Import(const std::filesystem::path &path, uint32_t flags);

		inline const Node &GetNode(uint32_t index) const { return mNodes[index]; }
		inline uint32_t GetNodeCount() const { return (uint32_t)mNodes.size(); }

	private:
		std::vector<Node> mNodes;
	};

}