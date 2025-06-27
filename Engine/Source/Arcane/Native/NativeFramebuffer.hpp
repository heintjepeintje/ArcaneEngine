#pragma once

#include <Arcane/Core.hpp>
#include "NativeGraphicsContext.hpp"
#include "NativeTexture.hpp"

namespace Arcane {

	struct FramebufferInfo {
		uint32_t Width, Height;
		uint32_t Samples;
		bool FixedSampleLocations;
		const ImageFormat *Attachments;
		size_t AttachmentCount;
	};

	class NativeFramebuffer {
	public:
		static Ref<NativeFramebuffer> Create(const Ref<NativeGraphicsContext> &context, const FramebufferInfo &info);

	public:
		NativeFramebuffer() { }
		virtual ~NativeFramebuffer() { }

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual Ref<NativeTexture> GetColorTexture(uint32_t index) = 0;
		virtual uint32_t GetColorAttachmentCount() const = 0;
		virtual bool HasDepthTexture() const = 0;
		virtual Ref<NativeTexture> GetDepthTexture() = 0;
		virtual bool HasStencilTexture() const = 0;
		virtual Ref<NativeTexture> GetStencilTexture() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetSampleCount() const = 0;
		virtual bool HasFixedSampleLocations() const = 0;

		virtual const ImageFormat *GetAttachments() const = 0;
		virtual size_t GetAttachmentCount() const = 0;

		virtual void Destroy() = 0;
		virtual bool IsValid() const = 0;
	};

}