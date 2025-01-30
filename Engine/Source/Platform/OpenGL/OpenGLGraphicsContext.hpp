#pragma once

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

		virtual uint32_t GetVersionMajor() const override;
		virtual uint32_t GetVersionMinor() const override;
		virtual uint32_t GetPatchLevel() const override;
		virtual std::string GetDeviceName() const override;

		inline Ref<NativeWindow> GetWindow() const { return mWindow; }

	private:
		Ref<NativeWindow> mWindow;
#ifdef _WIN32
		HGLRC mRenderContext;
		HDC mDeviceContext;
#endif
	};

}