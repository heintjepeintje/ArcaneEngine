#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {
	
	struct Tag {
		Tag() { }
		Tag(const std::string &tag) : Name(tag) { }
	
		std::string Name;
	};

}