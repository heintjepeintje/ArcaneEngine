#include "StringUtils.hpp"

namespace Arcane {

	std::vector<std::string> SplitString(const std::string &str, char delim) {
		std::vector<std::string> tokens;
		std::string token = "";
		for (char c : str) {
			if (c == delim) {
				tokens.push_back(token);
				token = "";
			} else {
				token += c;
			}
		}
		tokens.push_back(token);
		return tokens;
	}

}
