#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Data/BufferData.hpp>

namespace Arcane {

	std::string ReadFile(const std::string &path);
	BufferRef ReadFileBinary(const std::string &path);
	
}