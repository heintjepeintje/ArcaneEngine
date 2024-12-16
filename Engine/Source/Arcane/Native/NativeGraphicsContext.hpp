#pragma once

#include <Arcane/Core.hpp>
#include "NativeWindow.hpp"

namespace Arcane {

	class NativeGraphicsContext {
	public:
		static std::shared_ptr<NativeGraphicsContext> Create(const std::shared_ptr<NativeWindow> &window);

	public:
		NativeGraphicsContext() { }
		virtual ~NativeGraphicsContext() { }

		virtual void Present() = 0;
		
		virtual uint32_t GetVersionMajor() const = 0;
		virtual uint32_t GetVersionMinor() const = 0;
		virtual uint32_t GetPatchLevel() const = 0;
	};

}