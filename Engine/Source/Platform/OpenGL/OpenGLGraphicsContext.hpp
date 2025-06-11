#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeGraphicsContext.hpp>
#include <Arcane/Native/NativeWindow.hpp>

#ifdef _WIN32
#	include <Platform/Windows/WindowsCore.hpp>
#endif

namespace Arcane {

	class OpenGLGraphicsContext : public NativeGraphicsContext {
	public:
		OpenGLGraphicsContext(const Ref<NativeWindow> &window);
		~OpenGLGraphicsContext();

		virtual void Present() override;

		virtual uint32_t GetVersionMajor() const override { return mMajorVersion; }
		virtual uint32_t GetVersionMinor() const override { return mMinorVersion; }
		virtual uint32_t GetPatchLevel() const override { return 0; }
		virtual const char *GetDeviceName() const override { return mDeviceName; }
		virtual Ref<NativeWindow> GetWindow() const override { return mWindow; }
		virtual const GraphicsLimits &GetGraphicsLimits() const override { return mLimits; }

		inline virtual GraphicsAPI GetGraphicsAPI() const override { return GraphicsAPI::OpenGL; }

	private:
		Ref<NativeWindow> mWindow;
		GraphicsLimits mLimits;
		uint32_t mMajorVersion, mMinorVersion;
		char *mDeviceName;
#ifdef _WIN32
		HGLRC mRenderContext;
		HDC mDeviceContext;
#endif
	};

}