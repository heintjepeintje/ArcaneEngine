#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeWindow.hpp>

namespace Arcane {

	class Window {
	public:
		static Window Create(uint32_t width, uint32_t height, const std::string &title);

	public:
		Window() { }
		~Window() { }

		inline void SetVisible(bool visible) { mNativeWindow->SetVisible(visible); }
		inline void SetMaximized(bool maximized) { mNativeWindow->SetMaximized(maximized); }
		inline void SetTitle(const char *title) { mNativeWindow->SetTitle(title); }

		inline void Update() { mNativeWindow->Update(); }
		inline void Destroy() { mNativeWindow->Destroy(); }

		inline bool IsVisible() const { return mNativeWindow->IsVisible(); }
		inline bool IsMaximized() const { return mNativeWindow->IsMaximized(); }
		inline bool IsClosed() const { return mNativeWindow->IsClosed(); }
		inline bool IsFocused() const { return mNativeWindow->IsFocused(); }

		inline Vector2 GetClientSize() const { return mNativeWindow->GetClientSize(); }
		inline Vector2 GetScreenSize() const { return mNativeWindow->GetScreenSize(); }

		inline Ref<NativeWindow> GetNativeWindow() const { return mNativeWindow; }

	private:
		Window(const Ref<NativeWindow> &window) : mNativeWindow(window) { }

	private:
		Ref<NativeWindow> mNativeWindow;
	};

}