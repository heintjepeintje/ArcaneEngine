#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Math/Vector2.hpp>

namespace Arcane {

	enum class KeyCode : uint16_t {
		None = 0,

		Space = ' ',
		Tab, Escape, Backspace, Delete, Enter,

		Zero = '0',
		One, Two, Three, Four, Five, Six, Seven, Eight, Nine,
		
		A = 'A',
		B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		
		LeftShift, RightShift,
		LeftCtrl, RightCtrl,
		LeftAlt, RightAlt,

		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

		KeyCount
	};

	enum class MouseButton : uint16_t {
		None = 0,
		
		Left, Middle, Right,

		ButtonCount
	};

	uint32_t _ToNativeKeyCode(KeyCode keycode);
	uint32_t _ToNativeMouseButton(MouseButton button);
	KeyCode _ToArcaneKeyCode(uint32_t keycode);
	MouseButton _ToArcaneMouseButton(uint32_t button);

	bool _IsKeyPressed(uint32_t keycode);
	bool _IsMouseButtonPressed(uint32_t keycode);
	Vector2 _GetMousePosition();

	void _SetCursorPos(uint32_t x, uint32_t y);
	void _SetCursorVisible(bool visible);
	bool _IsCursorVisible();

}