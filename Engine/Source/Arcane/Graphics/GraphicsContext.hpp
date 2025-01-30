#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/System/Window.hpp>
#include <Arcane/Native/NativeGraphicsContext.hpp>

namespace Arcane {

	class GraphicsContext {
	public:
		static GraphicsContext Create(const Window &window);

	public:
		GraphicsContext() { }
		~GraphicsContext() { }

		inline void Present() { mNativeContext->Present(); }
		
		inline uint32_t GetVersionMajor() const { return GetNativeContext()->GetVersionMajor(); }
		inline uint32_t GetVersionMinor() const { return GetNativeContext()->GetVersionMinor(); }
		inline std::string GetDeviceName() const { return GetNativeContext()->GetDeviceName(); }
		inline uint32_t GetPatchLevel() const { return GetNativeContext()->GetPatchLevel(); }

		inline Ref<NativeGraphicsContext> GetNativeContext() const {
			AR_ASSERT(mNativeContext, "Native context is invalid");
			return mNativeContext;
		}

		inline Window GetWindow() const { return mWindow; }

	private:
		GraphicsContext(const Ref<NativeGraphicsContext> &context) : mNativeContext(context) { }

	private:
		Ref<NativeGraphicsContext> mNativeContext;
		Window mWindow;
	};

}