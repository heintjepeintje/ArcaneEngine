#pragma once

#include <Arcane/Asset/Importer.hpp>
#include <vector>

namespace Arcane {

	class GLBImporter : public Importer {
	public:
		GLBImporter(const std::string &path);
		~GLBImporter();

		virtual bool Import(uint32_t flags) override;

		virtual const Node &GetNode(uint32_t index) const override { return mNodes[index]; }
		virtual uint32_t GetNodeCount() const override { return mNodes.size(); }

	private:
		std::string mPath;
		std::vector<Node> mNodes;
	};

}