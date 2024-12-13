#include <Arcane/System/Window.hpp>

#include <iostream>

int main(int argc, char **argv) {
	Arcane::Window window = Arcane::Window::Create(1920 / 2, 1080 / 2, "Arcane Engine");
	window.SetVisible(true);
	window.SetMaximized(true);

	while (!window.IsClosed()) {
		window.Update();
	}

	std::cout << "Closing" << std::endl;
	std::cin.get();

	return 0;
}