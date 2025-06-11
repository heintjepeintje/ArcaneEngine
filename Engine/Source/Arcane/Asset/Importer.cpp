#include "Importer.hpp"

#include <Arcane/Asset/Importers/GLBImporter.hpp>

namespace Arcane {

	Importer::Importer() { }

	Importer::~Importer() { }

	bool Importer::Import(const std::filesystem::path &path, uint32_t flags) {
		if (path.extension() == ".glb") {
			return ImportGLB(path, flags, mNodes);
		}
		return false;
	}

}