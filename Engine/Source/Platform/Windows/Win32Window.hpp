#pragma once

#ifdef _WIN32

#include "WindowsCore.hpp"
#include <Arcane/Native/NativeWindow.hpp>

namespace Arcane {

	struct Win32WindowData {
		bool isClosed = false;
	};

	class Win32Window : public NativeWindow {
	public:
		Win32Window(uint32_t width, uint32_t height, const std::string &title);
		~Win32Window();

		virtual void SetVisible(bool visible) override;
		virtual void SetMaximized(bool maximized) override;
		virtual void SetTitle(const char *title) override;

		virtual void Update() override;

		virtual void Destroy() override;

		virtual bool IsVisible() const override;
		virtual bool IsMaximized() const override;
		virtual bool IsClosed() const override;
		virtual bool IsFocused() const override;

		virtual Vector2 GetClientSize() const override;
		virtual Vector2 GetScreenSize() const override;

		inline HWND GetHWND() const { return mHandle; }

	private:
		Win32WindowData mData;
		HWND mHandle;
	};

}

#endif // _WIN32