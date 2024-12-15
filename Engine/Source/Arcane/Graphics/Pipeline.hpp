#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Native/NativePipeline.hpp>
#include <Arcane/Graphics/GraphicsContext.hpp>

namespace Arcane {

	class Pipeline {
	public:
		static Pipeline Create(const GraphicsContext &context, const PipelineInfo &info);
	public:
		Pipeline() { }
		~Pipeline() { }

	private:
		Pipeline(const std::shared_ptr<NativePipeline> &pipeline) : mNativePipeline(pipeline) { }
	
	private:
		std::shared_ptr<NativePipeline> mNativePipeline;
	};

}