#pragma once

#include <Arcane/Core.hpp>
#include "GraphicsContext.hpp"
#include "Buffer.hpp"
#include "Texture.hpp"
#include <Arcane/Native/NativePipeline.hpp>

namespace Arcane {

	class Pipeline {
	public:
		static Pipeline Create(const GraphicsContext &context, const PipelineInfo &info);
	public:
		Pipeline() { }
		Pipeline(const Ref<NativePipeline> &pipeline) : mNativePipeline(pipeline) { }
		~Pipeline() { }

		inline CullMode GetCullMode() const { return GetNativePipeline()->GetCullMode(); }
		inline WindingOrder GetWindingOrder() const { return GetNativePipeline()->GetWindingOrder(); }
		inline FillMode GetFillMode() const { return GetNativePipeline()->GetFillMode(); }
		inline PrimitiveTopology GetTopology() const { return GetNativePipeline()->GetTopology(); }
		inline InputLayout GetLayout() const { return GetNativePipeline()->GetLayout(); }
		inline Rect2D GetViewport() const { return GetNativePipeline()->GetViewport(); }
		inline Rect2D GetScissor() const { return GetNativePipeline()->GetScissor(); }
		inline size_t GetElementSize() const { return GetNativePipeline()->GetElementSize(); }
		inline uint32_t GetSampleCount() const { return GetNativePipeline()->GetSampleCount(); }
		inline uint8_t GetOutputMask() const { return GetNativePipeline()->GetOutputMask(); }
		inline float GetLineWidth() const { return GetNativePipeline()->GetLineWidth(); }
		inline float GetPointSize() const { return GetNativePipeline()->GetPointSize(); }
		inline InputLayout GetInputLayout() const { return GetNativePipeline()->GetInputLayout(); }
		inline float GetPolygonOffsetFactor() const { return GetNativePipeline()->GetPolygonOffsetFactor(); }
		inline float GetPolygonOffsetUnits() const { return GetNativePipeline()->GetPolygonOffsetUnits(); }

		inline void SetUniformBuffer(uint32_t index, const Buffer &buffer) { GetNativePipeline()->SetUniformBuffer(index, buffer.GetNativeBuffer()); }
		inline void SetCombinedImageSampler(uint32_t index, const Texture &texture, const Sampler &sampler) { GetNativePipeline()->SetCombinedImageSampler(index, texture.GetNativeTexture(), sampler.GetNativeSampler()); }

		inline Ref<NativePipeline> GetNativePipeline() const {
			AR_ASSERT(mNativePipeline, "Native pipeline is invalid");
			return mNativePipeline;
		}
	
	private:
		Ref<NativePipeline> mNativePipeline;
	};

}