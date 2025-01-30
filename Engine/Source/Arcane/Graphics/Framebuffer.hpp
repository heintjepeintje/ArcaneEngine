#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativeFramebuffer.hpp>
#include "GraphicsContext.hpp"
#include "Texture.hpp"

namespace Arcane {

	class Framebuffer {
	public:
		static Framebuffer Create(const GraphicsContext &context, const FramebufferInfo &info);

	public:
		Framebuffer() { }
		~Framebuffer() { }

		inline void Resize(uint32_t width, uint32_t height) { GetNativeFramebuffer()->Resize(width, height); }
		inline void Resize(const Vector2 &size) { Resize(size.X, size.Y); }

		inline Vector2 GetSize() const { return Vector2(GetNativeFramebuffer()->GetWidth(), GetNativeFramebuffer()->GetHeight()); }
		inline uint32_t GetWidth() const { return GetNativeFramebuffer()->GetWidth(); }
		inline uint32_t GetHeight() const { return GetNativeFramebuffer()->GetHeight(); }

		inline Texture GetColorBuffer(uint32_t index) { return Texture(GetNativeFramebuffer()->GetColorBuffer(index)); }
		inline Texture GetDepthBuffer() { return Texture(GetNativeFramebuffer()->GetDepthBuffer()); }
		inline Texture GetStencilBuffer() { return Texture(GetNativeFramebuffer()->GetStencilBuffer()); }

		inline const Attachment *GetAttachments() const { return GetNativeFramebuffer()->GetAttachments(); }

		inline Ref<NativeFramebuffer> GetNativeFramebuffer() const {
			AR_ASSERT(mNativeFramebuffer, "Native framebuffer is invalid");
			return mNativeFramebuffer;
		}
	private:
		Framebuffer(const Ref<NativeFramebuffer> &framebuffer) : mNativeFramebuffer(framebuffer) { }

	private:
		Ref<NativeFramebuffer> mNativeFramebuffer;
	};

}