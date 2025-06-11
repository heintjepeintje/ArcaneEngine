#pragma once

#include <Arcane/Core.hpp>
#include "NativeFramebuffer.hpp"
#include "NativeGraphicsContext.hpp"
#include "NativePipeline.hpp"

namespace Arcane {

	class NativeRenderPass {
	public:
		static Ref<NativeRenderPass> Create(const Ref<NativeGraphicsContext> &context, const Ref<NativePipeline> &pipeline, const ImageFormat *attachments, size_t attachmentCount);

	public:
		NativeRenderPass() { }
		virtual ~NativeRenderPass() { }

		virtual Ref<NativePipeline> GetPipeline() const = 0;
		virtual const ImageFormat *GetAttachments() const = 0;
		virtual size_t GetAttachmentCount() const = 0;
	};

}