#pragma once

#include <Arcane/Core.hpp>
#include "NativeWindow.hpp"

namespace Arcane {

	class NativeGraphicsContext {
	public:
		static std::shared_ptr<NativeGraphicsContext> Create(const std::shared_ptr<NativeWindow> &window);

	public:
		NativeGraphicsContext() { }
		virtual ~NativeGraphicsContext() { }

		virtual void SwapBuffers() = 0;
	};

}