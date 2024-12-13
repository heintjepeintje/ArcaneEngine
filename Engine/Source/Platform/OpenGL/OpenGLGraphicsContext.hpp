#pragma once

#include <Arcane/Native/NativeGraphicsContext.hpp>
#include <Arcane/Native/NativeWindow.hpp>

#ifdef _WIN32
#	include <Platform/Windows/WindowsCore.hpp>
#endif

namespace Arcane {

	class OpenGLGraphicsContext : public NativeGraphicsContext {
	public:
		OpenGLGraphicsContext(const std::shared_ptr<NativeWindow> &window);
		~OpenGLGraphicsContext();

		virtual void SwapBuffers() override;

	private:
		std::shared_ptr<NativeWindow> mWindow;
#ifdef _WIN32
		HGLRC mRenderContext;
		HDC mDeviceContext;
#endif
	};

}