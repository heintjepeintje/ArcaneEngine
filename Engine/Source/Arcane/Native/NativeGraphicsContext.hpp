#pragma once

#include <Arcane/Core.hpp>
#include "NativeWindow.hpp"
#include <Arcane/Math/Rect2D.hpp>

namespace Arcane {

	class NativeGraphicsContext {
	public:
		static Ref<NativeGraphicsContext> Create(const Ref<NativeWindow> &window);

	public:
		NativeGraphicsContext() { }
		virtual ~NativeGraphicsContext() { }

		virtual void Present() = 0;
		
		virtual uint32_t GetVersionMajor() const = 0;
		virtual uint32_t GetVersionMinor() const = 0;
		virtual uint32_t GetPatchLevel() const = 0;
		virtual std::string GetDeviceName() const = 0;
	};

}