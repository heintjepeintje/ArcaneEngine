#pragma once

#include <Arcane/Asset/Importer.hpp>
#include <vector>

namespace Arcane {

	bool ImportGLB(const std::filesystem::path &path, uint32_t flags, std::vector<Node> &nodes);

}