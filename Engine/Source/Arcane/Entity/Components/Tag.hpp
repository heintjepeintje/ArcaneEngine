#pragma once

#include <Arcane/Core.hpp>

namespace Arcane {

	class Tag {
	public:
		Tag() : mTag("") { }
		Tag(const std::string &tag) : mTag(tag) { }
		Tag(const char *tag) : mTag(tag) { }
		~Tag() = default;

		inline std::string GetTag() const { return mTag; }

	private:
		std::string mTag;
	};

}