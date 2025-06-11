#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/System/Window.hpp>
#include <Arcane/Native/NativeGraphicsContext.hpp>

namespace Arcane {

	class GraphicsContext {
	public:
		static GraphicsContext Create(const Window &window, GraphicsAPI api);
		static GraphicsContext &GetCurrent();
		static void SetCurrent(const GraphicsContext &context);

	public:
		GraphicsContext() { }
		GraphicsContext(const Ref<NativeGraphicsContext> &context) : mNativeContext(context) { }
		~GraphicsContext() { }

		inline void Present() { mNativeContext->Present(); }
		
		inline uint32_t GetVersionMajor() const { return GetNativeContext()->GetVersionMajor(); }
		inline uint32_t GetVersionMinor() const { return GetNativeContext()->GetVersionMinor(); }
		inline const char *GetDeviceName() const { return GetNativeContext()->GetDeviceName(); }
		inline uint32_t GetPatchLevel() const { return GetNativeContext()->GetPatchLevel(); }
		inline Window GetWindow() const { return Window(GetNativeContext()->GetWindow()); }
		inline const GraphicsLimits &GetGraphicsLimits() const { return GetNativeContext()->GetGraphicsLimits(); }

		inline Ref<NativeGraphicsContext> GetNativeContext() const {
			AR_ASSERT(mNativeContext, "Native context is invalid");
			return mNativeContext;
		}

	private:
		Ref<NativeGraphicsContext> mNativeContext;
	};

}