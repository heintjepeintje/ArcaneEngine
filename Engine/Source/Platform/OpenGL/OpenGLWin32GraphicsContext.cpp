#ifdef _WIN32

#include <Platform/Windows/Win32Window.hpp>
#include "OpenGLGraphicsContext.hpp"
#include "OpenGLCore.hpp"

#include <Arcane/Util/StringUtils.hpp>

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB 0x2093
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

#define WGL_ERROR_INVALID_VERSION_ARB 0x2095
#define WGL_ERROR_INVALID_PROFILE_ARB 0x2096

#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_DRAW_TO_BITMAP_ARB 0x2002
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_NEED_PALETTE_ARB 0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB 0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB 0x2006
#define WGL_SWAP_METHOD_ARB 0x2007
#define WGL_NUMBER_OVERLAYS_ARB 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB 0x2009
#define WGL_TRANSPARENT_ARB 0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB 0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define WGL_SHARE_DEPTH_ARB 0x200C
#define WGL_SHARE_STENCIL_ARB 0x200D
#define WGL_SHARE_ACCUM_ARB 0x200E
#define WGL_SUPPORT_GDI_ARB 0x200F
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_STEREO_ARB 0x2012
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_RED_BITS_ARB 0x2015
#define WGL_RED_SHIFT_ARB 0x2016
#define WGL_GREEN_BITS_ARB 0x2017
#define WGL_GREEN_SHIFT_ARB 0x2018
#define WGL_BLUE_BITS_ARB 0x2019
#define WGL_BLUE_SHIFT_ARB 0x201A
#define WGL_ALPHA_BITS_ARB 0x201B
#define WGL_ALPHA_SHIFT_ARB 0x201C
#define WGL_ACCUM_BITS_ARB 0x201D
#define WGL_ACCUM_RED_BITS_ARB 0x201E
#define WGL_ACCUM_GREEN_BITS_ARB 0x201F
#define WGL_ACCUM_BLUE_BITS_ARB 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB 0x2021
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_AUX_BUFFERS_ARB 0x2024

#define WGL_NO_ACCELERATION_ARB 0x2025
#define WGL_GENERIC_ACCELERATION_ARB 0x2026
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_SWAP_EXCHANGE_ARB 0x2028
#define WGL_SWAP_COPY_ARB 0x2029
#define WGL_SWAP_UNDEFINED_ARB 0x202A
#define WGL_TYPE_RGBA_ARB 0x202B
#define WGL_TYPE_COLORINDEX_ARB 0x202C

namespace Arcane {

	void OnOpenGLDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH: AR_OPENGL_ASSERT(false, "%s", message); break;
			case GL_DEBUG_SEVERITY_MEDIUM: AR_OPENGL_ERROR("%s", message); break;
			// case GL_DEBUG_SEVERITY_LOW: AR_OPENGL_WARNING("%s", message); break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: AR_OPENGL_INFO("%s", message); break;
		}
	}

	OpenGLGraphicsContext::OpenGLGraphicsContext(const Ref<NativeWindow> &window) : mWindow(window) {
		AR_PROFILE_FUNCTION();
		Ref<Win32Window> win32Window = CastRef<Win32Window>(window);

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

		typedef void(*PFNWGLSWAPINTERVALEXTPROC)(int);
		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

		LoadGL();

		GLint extensionCount = 0;
		glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);
		for (int32_t i = 0; i < extensionCount; i++) {
			const GLubyte *extension = glGetStringi(GL_EXTENSIONS, i);
			if (strcmp((const char *)extension, "WGL_EXT_swap_control ") == 0) {
				wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
			}
		}

		// if (wglSwapIntervalEXT) {
		// 	wglSwapIntervalEXT(0);
		// }

		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		glDebugMessageCallback(OnOpenGLDebugMessage, nullptr);

		LoadGLExtensions();
		
		AR_PROFILE_GPU_CONTEXT("Main OpenGL Context");

		glGetIntegerv(GL_MAJOR_VERSION, (GLint*)&mMajorVersion);
		glGetIntegerv(GL_MINOR_VERSION, (GLint*)&mMinorVersion);
		AR_ASSERT(mMajorVersion >= 4 && mMinorVersion >= 6, "Only OpenGL 4.6 or higher is supported. (current: %u.%u)\n", mMajorVersion, mMinorVersion);

		const GLubyte *deviceName = glGetString(GL_RENDERER);
		size_t length = strlen((const char *)deviceName);
		mDeviceName = new char[length + 1];
		std::memset(mDeviceName, 0, length + 1);
		std::memcpy(mDeviceName, deviceName, length);

		glGetIntegerv(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, (GLint*)&mLimits.MaxShaderStorageBlocks);
		glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, (GLint*)&mLimits.MaxUniformBlocks);
		glGetIntegerv(GL_MAX_COMBINED_IMAGE_UNIFORMS, (GLint*)&mLimits.MaxTextureImageUnits);
		
		glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, (GLint*)&mLimits.MaxWorkGroupInvocations);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, (GLint*)&mLimits.MaxWorkGroupCountX);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, (GLint*)&mLimits.MaxWorkGroupCountY);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, (GLint*)&mLimits.MaxWorkGroupCountZ);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, (GLint*)&mLimits.MaxWorkGroupSizeX);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, (GLint*)&mLimits.MaxWorkGroupSizeY);
		glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, (GLint*)&mLimits.MaxWorkGroupSizeZ);

		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, (GLint*)&mLimits.MaxVertexAttributes);

		glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, (GLint*)&mLimits.MaxCubeMapTextureSize);
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, (GLint*)&mLimits.Max3DTextureSize);
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint*)&mLimits.MaxTextureSize);
		glGetIntegerv(GL_MAX_TEXTURE_LOD_BIAS, (GLint*)&mLimits.MaxTextureLODBias);
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, (GLint*)&mLimits.MaxArrayTextureLayers);
		glGetIntegerv(GL_MAX_COLOR_TEXTURE_SAMPLES, (GLint*)&mLimits.MaxColorSamples);
		glGetIntegerv(GL_MAX_DEPTH_TEXTURE_SAMPLES, (GLint*)&mLimits.MaxDepthSamples);

		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, (GLint*)&mLimits.MaxUniformBlockSize);
		glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, (GLint*)&mLimits.MaxStorageBlockSize);

		glGetIntegerv(GL_MAX_CLIP_DISTANCES, (GLint*)&mLimits.MaxClipDistances);
		
		uint32_t viewportSize[2];
		glGetIntegerv(GL_MAX_VIEWPORT_DIMS, (GLint*)viewportSize);
		mLimits.MaxViewportWidth = viewportSize[0];
		mLimits.MaxViewportHeight = viewportSize[1];

		glGetIntegerv(GL_MAX_VIEWPORTS, (GLint*)&mLimits.MaxViewports);
		glGetIntegerv(GL_MAX_DRAW_BUFFERS, (GLint*)&mLimits.MaxDrawBuffers);

		glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, (GLint*)&mLimits.MaxFramebufferWidth);
		glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, (GLint*)&mLimits.MaxFramebufferHeight);
		glGetIntegerv(GL_MAX_FRAMEBUFFER_LAYERS, (GLint*)&mLimits.MaxFramebufferLayers);
		glGetIntegerv(GL_MAX_FRAMEBUFFER_SAMPLES, (GLint*)&mLimits.MaxFramebufferSamples);
	}

	OpenGLGraphicsContext::~OpenGLGraphicsContext() {
		AR_PROFILE_FUNCTION();
		delete[] mDeviceName;
		if (wglGetCurrentContext() == mRenderContext) {
			wglMakeCurrent(mDeviceContext, NULL);
		}
		wglDeleteContext(mRenderContext);
	}

	void OpenGLGraphicsContext::Present() {
		AR_PROFILE_FUNCTION();
		wglSwapLayerBuffers(mDeviceContext, WGL_SWAP_MAIN_PLANE);
		AR_PROFILE_GPU_COLLECT();
	}

}
#endif