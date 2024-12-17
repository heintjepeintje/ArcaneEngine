#pragma once

#include <Arcane/Core.hpp>
#include "GraphicsContext.hpp"
#include "Buffer.hpp"
#include <Arcane/Native/NativePipeline.hpp>

namespace Arcane {

	class Pipeline {
	public:
		static Pipeline Create(const GraphicsContext &context, const PipelineInfo &info);
	public:
		Pipeline() { }
		~Pipeline() { }

		inline CullMode GetCullMode() const { return mNativePipeline->GetCullMode(); }
		inline WindingOrder GetWindingOrder() const { return mNativePipeline->GetWindingOrder(); }
		inline FillMode GetFillMode() const { return mNativePipeline->GetFillMode(); }
		inline PrimitiveTopology GetTopology() const { return mNativePipeline->GetTopology(); }
		inline InputLayout GetLayout() const { return mNativePipeline->GetLayout(); }
		inline Rect2D GetViewport() const { return mNativePipeline->GetViewport(); }
		inline Rect2D GetScissor() const { return mNativePipeline->GetScissor(); }

		inline void SetDescriptor(uint32_t index, const Buffer &buffer) { mNativePipeline->SetDescriptor(index, buffer.GetNativeBuffer()); }

		inline std::shared_ptr<NativePipeline> GetNativePipeline() const { return mNativePipeline; }

	private:
		Pipeline(const std::shared_ptr<NativePipeline> &pipeline) : mNativePipeline(pipeline) { }
	
	private:
		std::shared_ptr<NativePipeline> mNativePipeline;
	};

}