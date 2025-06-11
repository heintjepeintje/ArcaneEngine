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
		inline uint32_t GetSampleCount() const { return GetNativeFramebuffer()->GetSampleCount(); }
		inline bool HasFixedSampleLocations() const { return GetNativeFramebuffer()->HasFixedSampleLocations(); }

		inline Texture GetColorTexture(uint32_t index) const { return Texture(GetNativeFramebuffer()->GetColorTexture(index)); }
		inline uint32_t GetColorAttachmentCount() const { return GetColorAttachmentCount(); }
		inline Texture GetDepthTexture() const { return Texture(GetNativeFramebuffer()->GetDepthTexture()); }
		inline Texture GetStencilTexture() const { return Texture(GetNativeFramebuffer()->GetStencilTexture()); }

		inline const ImageFormat *GetAttachments() const { return GetNativeFramebuffer()->GetAttachments(); }
		inline uint32_t GetAttachmentCount() const { return GetNativeFramebuffer()->GetAttachmentCount(); }

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