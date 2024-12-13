#include <Arcane/System/Window.hpp>
#include <Arcane/Graphics/GraphicsContext.hpp>

#include <iostream>

int main(int argc, char **argv) {
	Arcane::Window window = Arcane::Window::Create(1920 / 2, 1080 / 2, "Arcane Engine");
	window.SetVisible(true);
	window.SetMaximized(true);

	Arcane::GraphicsContext context = Arcane::GraphicsContext::Create(window);

	while (!window.IsClosed()) {
		context.SwapBuffers();
		window.Update();
	}

	window.Destroy();

	return 0;
}