#ifdef _WIN32

#include <Arcane/Native/NativeInput.hpp>
#include "WindowsCore.hpp"

namespace Arcane {

	uint32_t _ToNativeKeyCode(KeyCode keycode) {
		uint32_t ukey = (uint32_t)keycode;
		if (ukey >= 'A' && ukey <= 'Z') {
			return (uint32_t)ukey;
		}

		if (ukey >= '0' && ukey <= '9') {
			return (uint32_t)ukey;
		}

		if (ukey >= (uint32_t)KeyCode::F1 && ukey <= (uint32_t)KeyCode::F12) {
			return ukey - (uint32_t)KeyCode::F1 + VK_F1;
		}

		switch (keycode) {
			case KeyCode::Space: return VK_SPACE;
			case KeyCode::Tab: return VK_TAB;
			case KeyCode::Escape: return VK_ESCAPE;
			case KeyCode::Backspace: return VK_BACK;
			case KeyCode::Delete: return VK_DELETE;
			case KeyCode::Enter: return VK_RETURN;
			case KeyCode::LeftShift: return VK_LSHIFT;
			case KeyCode::RightShift: return VK_RSHIFT;
			case KeyCode::LeftCtrl: return VK_LCONTROL;
			case KeyCode::RightCtrl: return VK_RCONTROL;
			case KeyCode::LeftAlt: return VK_LMENU;
			case KeyCode::RightAlt: return VK_RMENU;
			default: return 0;
		}
	}

	uint32_t _ToNativeMouseButton(MouseButton button) {
		switch (button) {
			case MouseButton::Left: return VK_LBUTTON;
			case MouseButton::Middle: return VK_MBUTTON;
			case MouseButton::Right: return VK_RBUTTON;
			default: return 0;
		}
	}

	KeyCode _ToArcaneKeyCode(uint32_t keycode) {
		if (keycode >= 'A' && keycode <= 'Z') {
			return (KeyCode)keycode;
		}

		if (keycode >= '0' && keycode <= '9') {
			return (KeyCode)keycode;
		}

		if (keycode >= VK_F1 && keycode <= VK_F12) {
			return (KeyCode)(keycode - VK_F1 + (uint32_t)KeyCode::F1);
		}

		switch (keycode) {
			case VK_SPACE: return KeyCode::Space;
			case VK_TAB: return KeyCode::Tab;
			case VK_ESCAPE: return KeyCode::Escape;
			case VK_BACK: return KeyCode::Backspace;
			case VK_DELETE: return KeyCode::Delete;
			case VK_RETURN: return KeyCode::Enter;
			case VK_LSHIFT: return KeyCode::LeftShift;
			case VK_RSHIFT: return KeyCode::RightShift;
			case VK_LCONTROL: return KeyCode::LeftCtrl;
			case VK_RCONTROL: return KeyCode::RightCtrl;
			case VK_LMENU: return KeyCode::LeftAlt;
			case VK_RMENU: return KeyCode::RightAlt;
			default: return KeyCode::None;
		}
	}

	MouseButton _ToArcaneMouseButton(uint32_t button) {
		switch (button) {
			case VK_LBUTTON: return MouseButton::Left;
			case VK_MBUTTON: return MouseButton::Middle;
			case VK_RBUTTON: return MouseButton::Right;
			default: return MouseButton::None;
		}
	}

	bool _IsKeyPressed(uint32_t keycode) {
		return HIWORD(GetKeyState(keycode));
	}

	bool _IsMouseButtonPressed(uint32_t keycode) {
		return HIWORD(GetKeyState(keycode));
	}

	Vector2 _GetMousePosition() {
		POINT point;
		GetCursorPos(&point);
		return Vector2((float)point.x, (float)point.y);
	}

	void _SetCursorPos(uint32_t x, uint32_t y) {
		SetCursorPos(x, y);
	}

	void _SetCursorVisible(bool visible) {
		ShowCursor(visible);
	}

	bool _IsCursorVisible() {
		CURSORINFO info;
		GetCursorInfo(&info);
		return info.flags == CURSOR_SHOWING;
	}

}

#endif // _WIN32