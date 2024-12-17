#ifdef _WIN32

#include <Platform/Windows/Win32Window.hpp>
#include "OpenGLGraphicsContext.hpp"
#include "OpenGLCore.hpp"

namespace Arcane {

	void OnOpenGLDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
		AR_ASSERT(type != GL_DEBUG_TYPE_ERROR, "OpenGL > %*s\n", length, message);
	}

	OpenGLGraphicsContext::OpenGLGraphicsContext(const std::shared_ptr<NativeWindow> &window) : mWindow(window) {
		std::shared_ptr<Win32Window> win32Window = std::dynamic_pointer_cast<Win32Window>(window);

		PIXELFORMATDESCRIPTOR pfd = {};
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;

		mDeviceContext = GetDC(win32Window->GetHWND());
		int format = ChoosePixelFormat(mDeviceContext, &pfd);
		AR_ASSERT(format != 0, "Could not find valid pixel format\n");

		SetPixelFormat(mDeviceContext, format, &pfd);

		mRenderContext = wglCreateContext(mDeviceContext);
		wglMakeCurrent(mDeviceContext, mRenderContext);

		std::printf("Loading OpenGL...\n");
		LoadGL();

		glDebugMessageCallback(OnOpenGLDebugMessage, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	OpenGLGraphicsContext::~OpenGLGraphicsContext() {
		if (wglGetCurrentContext() == mRenderContext) {
			wglMakeCurrent(mDeviceContext, NULL);
		}
		wglDeleteContext(mRenderContext);
	}

	void OpenGLGraphicsContext::Present() {
		wglSwapLayerBuffers(mDeviceContext, WGL_SWAP_MAIN_PLANE);
	}

	uint32_t OpenGLGraphicsContext::GetVersionMajor() const {
		int32_t major = 0;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		return major;
	}

	uint32_t OpenGLGraphicsContext::GetVersionMinor() const {
		int32_t major = 0;
		glGetIntegerv(GL_MINOR_VERSION, &major);
		return major;
	}

	uint32_t OpenGLGraphicsContext::GetPatchLevel() const {
		return 0;
	}

	std::shared_ptr<NativeGraphicsContext> NativeGraphicsContext::Create(const std::shared_ptr<NativeWindow> &window) {
		return std::make_shared<OpenGLGraphicsContext>(
			window
		);
	}

}
#endif