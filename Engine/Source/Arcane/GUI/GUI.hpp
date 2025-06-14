#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/System/Window.hpp>
#include <Arcane/Graphics/Base/GraphicsContext.hpp>
#include <Arcane/Graphics/Base/RendererAPI.hpp>
#include <Arcane/Graphics/Base/Framebuffer.hpp>
#include <Arcane/Graphics/Base/Texture.hpp>
#include <imgui.h>

namespace Arcane {

	namespace GUI {

		void Init(const Window &window, const GraphicsContext &context, const RendererAPI &rendererAPI);
		void BeginFrame();
		void EndFrame();
		void Draw();

		Framebuffer &GetOutputFramebuffer();

		ImTextureID CreateTextureID(const Texture &texture);
		Texture &GetTextureFromID(ImTextureID id);

	}

}