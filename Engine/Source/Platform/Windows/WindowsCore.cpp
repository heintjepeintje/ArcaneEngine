#include "WindowsCore.hpp"

namespace Arcane {

	Logger &GetWindowsLogger() {
		static Logger logger("Arcane.Windows");
		return logger;
	}

	std::string GetWindowsErrorMessageString(int errorCode) {
		char *buffer;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			errorCode,
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
			reinterpret_cast<LPSTR>(&buffer),
			0,
			nullptr
		);
		std::string message(buffer);
		free(buffer);
		return message;
	}

}