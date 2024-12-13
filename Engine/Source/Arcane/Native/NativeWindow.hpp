#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Math/Vector.hpp>

namespace Arcane {

	class NativeWindow {
	public:
		static std::shared_ptr<NativeWindow> Create(uint32_t width, uint32_t height, const std::string &title);

	public:
		NativeWindow() { }
		virtual ~NativeWindow() { }

		virtual void SetVisible(bool visible) = 0;
		virtual void SetMaximized(bool maximized) = 0;

		virtual void Update() = 0;

		virtual void Destroy() = 0;

		virtual bool IsVisible() const = 0;
		virtual bool IsMaximized() const = 0;
		virtual bool IsClosed() const = 0;


		virtual Vector2 GetClientSize() const = 0;
		virtual Vector2 GetScreenSize() const = 0;
	};

}