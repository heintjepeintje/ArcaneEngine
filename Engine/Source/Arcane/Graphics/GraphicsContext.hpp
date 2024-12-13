#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/System/Window.hpp>
#include <Arcane/Native/NativeGraphicsContext.hpp>

namespace Arcane {

	class GraphicsContext {
	public:
		static GraphicsContext Create(const Window &window);

	public:
		GraphicsContext() { }
		~GraphicsContext() { }

		inline void SwapBuffers() { mNativeContext->SwapBuffers(); }

	private:
		GraphicsContext(const std::shared_ptr<NativeGraphicsContext> &context) : mNativeContext(context) { }

	private:
		std::shared_ptr<NativeGraphicsContext> mNativeContext;
	};

}