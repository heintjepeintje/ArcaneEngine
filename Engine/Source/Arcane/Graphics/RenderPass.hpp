#pragma once

#include <Arcane/Core.hpp>
#include "GraphicsContext.hpp"
#include "Attachment.hpp"
#include "Pipeline.hpp"
#include <Arcane/Native/NativeRenderPass.hpp>

namespace Arcane {

	class RenderPass {
	public:
		static RenderPass Create(const GraphicsContext& context, const Pipeline &pipeline, const Attachment *attachments, size_t attachmentCount);

	public:
		RenderPass() { }
		~RenderPass() { }

		inline Ref<NativeRenderPass> GetNativeRenderPass() const {
			AR_ASSERT(mNativeRenderPass, "Native render pass is invalid");
			return mNativeRenderPass;
		}

	private:
		RenderPass(const Ref<NativeRenderPass> &renderPass) : mNativeRenderPass(renderPass) { }

	private:
		Ref<NativeRenderPass> mNativeRenderPass;
	};

}