#include "Importer.hpp"

#include "Importers/GLBImporter.hpp"

namespace Arcane {

	Ref<Importer> Importer::Create(const std::filesystem::path &path) {
		std::string extension = path.extension().string();
		if (extension == ".glb") {
			return CastRef<Importer>(CreateRef<GLBImporter>(path.string()));
		} else {
			AR_ASSERT(false, "Could not find importer for extension %s", extension.c_str());
			return Ref<Importer>();
		}
	}

}