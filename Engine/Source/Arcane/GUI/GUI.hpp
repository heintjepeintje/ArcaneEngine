#pragma once

#include <Arcane/System/Window.hpp>
#include <Arcane/Graphics/GraphicsContext.hpp>

namespace Arcane {

	namespace GUI {

		void Init(const Window &window, const GraphicsContext &context);
		void BeginFrame();
		void EndFrame();

	}

}