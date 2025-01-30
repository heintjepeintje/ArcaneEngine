#include "Input.hpp"

#include <Arcane/Native/NativeInput.hpp>

namespace Arcane {

	static bool sLastKeyStates[AR_MAX_KEYS] = { };
	static bool sKeyStates[AR_MAX_KEYS] = { };
	static bool sMouseButtonStates[AR_MAX_BUTTONS] = { };
	static bool sLastMouseButtonStates[AR_MAX_BUTTONS] = { };
	
	static Vector2 sMousePosition;
	static Vector2 sLastMousePosition; 

	static bool sCursorLocked = false; 

	static const Vector2 sCenter = { 1920.0f / 2.0f, 1080.0f / 2.0f };

	void UpdateInput() {
		for (uint32_t i = 0; i < AR_MAX_KEYS; i++) {
			sLastKeyStates[i] = sKeyStates[i];
			sKeyStates[i] = _IsKeyPressed(_ToNativeKeyCode((KeyCode)i));
		}

		for (uint32_t i = 0; i < AR_MAX_BUTTONS; i++) {
			sLastMouseButtonStates[i] = sMouseButtonStates[i];
			sMouseButtonStates[i] = _IsMouseButtonPressed(_ToNativeMouseButton((MouseButton)i));
		}

		if (sCursorLocked) {
			sLastMousePosition = sCenter;
			sMousePosition = _GetMousePosition();
			_SetCursorPos((uint32_t)sCenter.X, (uint32_t)sCenter.Y);
		} else {
			sLastMousePosition = sMousePosition;
			sMousePosition = _GetMousePosition();
		}

	}

	bool IsKeyPressed(KeyCode key) {
		return sKeyStates[(uint32_t)key];
	}

	bool IsKeyDown(KeyCode key) {
		return sKeyStates[(uint32_t)key] && !sLastKeyStates[(uint32_t)key];
	}

	bool IsKeyUp(KeyCode key) {
		return !sKeyStates[(uint32_t)key] && sLastKeyStates[(uint32_t)key];
	}

	bool IsMouseButtonPressed(MouseButton button) {
		return sMouseButtonStates[(uint32_t)button];
	}

	bool IsMouseButtonDown(MouseButton button) {
		return sMouseButtonStates[(uint32_t)button] && !sLastMouseButtonStates[(uint32_t)button];
	}

	bool IsMouseButtonUp(MouseButton button) {
		return !sMouseButtonStates[(uint32_t)button] && sLastMouseButtonStates[(uint32_t)button];
	}

	Vector2 GetMousePosition() {
		return sMousePosition;
	}

	Vector2 GetMouseDelta() {
		return sMousePosition - sLastMousePosition;
	}

	void SetCursorVisible(bool visible) {
		_SetCursorVisible(visible);
	}

	void SetCursorLocked(bool locked) {
		sCursorLocked = locked;
	}

	bool IsCursorVisible() {
		return _IsCursorVisible();
	}

	bool IsCursorLocked() {
		return sCursorLocked;
	}

}