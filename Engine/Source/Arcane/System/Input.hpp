#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeInput.hpp>

#define AR_MAX_KEYS ((uint32_t)::Arcane::KeyCode::KeyCount - 1)
#define AR_MAX_BUTTONS ((uint32_t)::Arcane::MouseButton::ButtonCount - 1)

namespace Arcane {

	void UpdateInput();

	bool IsKeyPressed(KeyCode key);
	bool IsKeyDown(KeyCode key);
	bool IsKeyUp(KeyCode key);
	bool IsMouseButtonPressed(MouseButton button);
	bool IsMouseButtonDown(MouseButton button);
	bool IsMouseButtonUp(MouseButton button);
	
	Vector2 GetMousePosition();
	Vector2 GetMouseDelta();

	void SetCursorVisible(bool visible);
	void SetCursorLocked(bool locked);
	bool IsCursorVisible();
	bool IsCursorLocked();

}