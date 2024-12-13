#include "Window.hpp"

namespace Arcane {

	Window Window::Create(uint32_t width, uint32_t height, const std::string &title) {
		return Window(NativeWindow::Create(
			width, height, title
		));
	}

}