#include "RenderPass.hpp"

namespace Arcane {

	RenderPass RenderPass::Create(const GraphicsContext& context, const Pipeline &pipeline, const ImageFormat *attachments, size_t attachmentCount) {
		return RenderPass(NativeRenderPass::Create(
			context.GetNativeContext(),
			pipeline.GetNativePipeline(),
			attachments, 
			attachmentCount
		));
	}

}