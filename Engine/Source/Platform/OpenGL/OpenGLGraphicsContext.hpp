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

		virtual void Present() override;

		virtual uint32_t GetVersionMajor() const override;
		virtual uint32_t GetVersionMinor() const override;
		virtual uint32_t GetPatchLevel() const override;

		inline std::shared_ptr<NativeWindow> GetWindow() const { return mWindow; }

	private:
		std::shared_ptr<NativeWindow> mWindow;
#ifdef _WIN32
		HGLRC mRenderContext;
		HDC mDeviceContext;
#endif
	};

}