#pragma once

#include <Arcane/Core.hpp>
#include "NativeGraphicsContext.hpp"
#include "NativeTexture.hpp"

#include <Arcane/Graphics/Attachment.hpp>

namespace Arcane {

	struct FramebufferInfo {
		uint32_t Width, Height;
		const Attachment *Attachments;
		size_t AttachmentCount;
	};

	class NativeFramebuffer {
	public:
		static Ref<NativeFramebuffer> Create(const Ref<NativeGraphicsContext> &context, const FramebufferInfo &info);

	public:
		NativeFramebuffer() { }
		virtual ~NativeFramebuffer() { }

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual Ref<NativeTexture> GetColorBuffer(uint32_t index) = 0;
		virtual Ref<NativeTexture> GetDepthBuffer() = 0;
		virtual Ref<NativeTexture> GetStencilBuffer() = 0;

		inline virtual uint32_t GetWidth() const = 0;
		inline virtual uint32_t GetHeight() const = 0;

		virtual const Attachment *GetAttachments() const = 0;
		virtual size_t GetAttachmentCount() const = 0;
	};

}