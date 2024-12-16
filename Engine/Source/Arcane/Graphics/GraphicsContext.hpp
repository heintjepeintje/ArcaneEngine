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
		
		inline uint32_t GetVersionMajor() const { return mNativeContext->GetVersionMajor(); }
		inline uint32_t GetVersionMinor() const { return mNativeContext->GetVersionMinor(); }
		inline uint32_t GetPatchLevel() const { return mNativeContext->GetPatchLevel(); }

		inline std::shared_ptr<NativeGraphicsContext> GetNativeContext() const { return mNativeContext; }

	private:
		GraphicsContext(const std::shared_ptr<NativeGraphicsContext> &context) : mNativeContext(context) { }

	private:
		std::shared_ptr<NativeGraphicsContext> mNativeContext;
	};

}