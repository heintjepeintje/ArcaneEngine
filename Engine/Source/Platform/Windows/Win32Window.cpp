#ifdef _WIN32

#include "Win32Window.hpp"

#define AR_WINDOW_CLASS_NAME "ArcaneWindowClass"

namespace Arcane {

	static bool sIsWindowClassInitialized = false;

	LRESULT CALLBACK WindowProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
		Win32WindowData *data = reinterpret_cast<Win32WindowData*>(GetWindowLongPtrA(window, GWLP_USERDATA));

		if (message == WM_CLOSE) {
			data->isClosed = true;
			return 0;
		}

		return DefWindowProcA(window, message, wparam, lparam);
	}

	static void InitWindowClass() {
		WNDCLASSA wc = { };
		wc.lpszClassName = AR_WINDOW_CLASS_NAME;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.lpfnWndProc = WindowProc;
		
		RegisterClassA(&wc);

		sIsWindowClassInitialized = true;
	} 

	Win32Window::Win32Window(uint32_t width, uint32_t height, const std::string &title) {
		if (!sIsWindowClassInitialized) InitWindowClass();

		mHandle = CreateWindowExA(
			0,
			AR_WINDOW_CLASS_NAME,
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			width, height,
			nullptr, nullptr, nullptr, nullptr
		);

		if (mHandle == nullptr) {
			// ! Handle invalid window handle
		}

		mData.isClosed = false;

		SetWindowLongPtrA(mHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&mData));
	}

	Win32Window::~Win32Window() {
		DestroyWindow(mHandle);
	}

	void Win32Window::SetVisible(bool visible) {
		ShowWindow(mHandle, visible ? SW_SHOW : SW_HIDE);
	}

	void Win32Window::SetMaximized(bool maximized) {
		ShowWindow(mHandle, maximized ? SW_MAXIMIZE : SW_NORMAL);
	}
	
	void Win32Window::Update() {
		MSG message = { };
		while (PeekMessageA(&message, mHandle, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}
	}

	bool Win32Window::IsVisible() const {
		return IsWindowVisible(mHandle);
	}

	bool Win32Window::IsMaximized() const {
		WINDOWPLACEMENT placement = { };
		GetWindowPlacement(mHandle, &placement);
		return placement.showCmd == SW_MAXIMIZE;
	}

	bool Win32Window::IsClosed() const {
		return mData.isClosed;
	}

	Vector2 Win32Window::GetClientSize() const {
		RECT clientRect = { };
		GetClientRect(mHandle, &clientRect);
		return Vector2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	}

	Vector2 Win32Window::GetScreenSize() const {
		RECT screenRect = { };
		GetWindowRect(mHandle, &screenRect);
		return Vector2(screenRect.right - screenRect.left, screenRect.bottom - screenRect.top);
	}

	std::shared_ptr<NativeWindow> NativeWindow::Create(uint32_t width, uint32_t height, const std::string &title) {
		return std::make_shared<Win32Window>(width, height, title);
	}

}

#endif // _WIN32