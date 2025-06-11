#include "Pipeline.hpp"

namespace Arcane {

	Pipeline Pipeline::Create(const GraphicsContext &context, const PipelineInfo &info) {
		return Pipeline(NativePipeline::Create(
			context.GetNativeContext(), info
		));
	}

}