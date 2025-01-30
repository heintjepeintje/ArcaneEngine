#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	std::string ReadFile(const std::string &path);
	uint8_t *ReadFileBinary(const std::string &path, size_t *size);
	
}