#include "FileUtil.hpp"

namespace Arcane {

	std::string ReadFile(const std::string &path) {
		FILE *f = fopen(path.c_str(), "r");
		AR_ASSERT(f, "Could not open file: %s", path.c_str());


		long begin = ftell(f);
		fseek(f, 0, SEEK_END);
		long end = ftell(f);
		fseek(f, 0, SEEK_SET);

		std::string content((end - begin) + 1, '\0');
		fread(content.data(), 1, end - begin, f);
		fclose(f);

		return content;
	}
	
	uint8_t *ReadFileBinary(const std::string &path, size_t *size) {
		FILE *f = fopen(path.c_str(), "rb");
		AR_ASSERT(f, "Could not open file: %s", path.c_str());


		long begin = ftell(f);
		fseek(f, 0, SEEK_END);
		long end = ftell(f);
		fseek(f, 0, SEEK_SET);

		*size = (end - begin);

		uint8_t *buffer = (uint8_t*)malloc(*size);
		fread(buffer, 1, end - begin, f);
		fclose(f);

		return buffer;
	}

}