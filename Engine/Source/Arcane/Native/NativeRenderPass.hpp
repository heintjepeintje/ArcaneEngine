#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/Attachment.hpp>
#include "NativeGraphicsContext.hpp"
#include "NativePipeline.hpp"

namespace Arcane {

	class NativeRenderPass {
	public:
		static Ref<NativeRenderPass> Create(const Ref<NativeGraphicsContext> &context, const Ref<NativePipeline> &pipeline, const Attachment *attachments, size_t attachmentCount);

	public:
		NativeRenderPass() { }
		virtual ~NativeRenderPass() { }

		virtual Ref<NativePipeline> GetPipeline() const = 0;
		virtual const Attachment *GetAttachments() const = 0;
		virtual size_t GetAttachmentCount() const = 0;
	};

}