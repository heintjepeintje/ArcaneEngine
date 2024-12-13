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

		inline void SetVisible(bool visible) { mNativeHandle->SetVisible(visible); }
		inline void SetMaximized(bool maximized) { mNativeHandle->SetMaximized(maximized); }

		inline void Update() { mNativeHandle->Update(); }

		inline bool IsVisible() const { return mNativeHandle->IsVisible(); }
		inline bool IsMaximized() const { return mNativeHandle->IsMaximized(); }
		inline bool IsClosed() const { return mNativeHandle->IsClosed(); }

		inline Vector2 GetClientSize() const { return mNativeHandle->GetClientSize(); }
		inline Vector2 GetScreenSize() const { return mNativeHandle->GetScreenSize(); }

	private:
		Window(const std::shared_ptr<NativeWindow> &native) : mNativeHandle(native) { }

	private:
		std::shared_ptr<NativeWindow> mNativeHandle;
	};

}