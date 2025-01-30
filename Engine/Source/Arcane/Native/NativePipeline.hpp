#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/InputLayout.hpp>
#include <Arcane/Math/Rect2D.hpp>
#include "NativeGraphicsContext.hpp"
#include "NativeBuffer.hpp"
#include "NativeTexture.hpp"

namespace Arcane {

	enum class CullMode {
		None = 0,
		Front,
		Back,
		FrontAndBack
	};

	enum class WindingOrder {
		None = 0,
		Clockwise,
		CounterClockwise
	};

	enum class FillMode {
		None = 0,
		Solid,
		Wireframe,
		Points
	};

	enum class PrimitiveTopology {
		None = 0,
		TriangleList,
		TriangleStrip,
		LineList,
		LineStrip,
		PointList
	};

	enum class DescriptorType {
		None = 0,
		UniformBuffer,
		CombinedImageSampler
	};

	struct Descriptor {
		uint32_t Binding;
		DescriptorType Type;
	};

	struct PipelineInfo {
		PipelineInfo() { }
		~PipelineInfo() { }

		CullMode CullMode;
		WindingOrder WindingOrder;
		FillMode FillMode;
		PrimitiveTopology Topology;	

		InputLayout Layout;

		Rect2D Viewport;
		Rect2D Scissor;
		
		uint8_t *VertexShaderBinary;
		size_t VertexShaderSize;

		uint8_t *FragmentShaderBinary;
		size_t FragmentShaderSize;

		Descriptor *Descriptors;
		uint32_t DescriptorCount;

		uint32_t SampleCount;
	};

	class NativePipeline {
	public:
		static Ref<NativePipeline> Create(const Ref<NativeGraphicsContext> &context, const PipelineInfo &info);

	public:
		NativePipeline() { }
		virtual ~NativePipeline() { }

		virtual CullMode GetCullMode() const = 0;
		virtual WindingOrder GetWindingOrder() const = 0;
		virtual FillMode GetFillMode() const = 0;
		virtual PrimitiveTopology GetTopology() const = 0;
		virtual InputLayout GetLayout() const = 0;
		virtual Rect2D GetViewport() const = 0;
		virtual Rect2D GetScissor() const = 0;

		virtual uint32_t GetSampleCount() const = 0;

		virtual void SetUniformBuffer(uint32_t binding, const Ref<NativeBuffer> &uniformBuffer) = 0;
		virtual void SetCombinedImageSampler(uint32_t binding, const Ref<NativeTexture> &texture, const Ref<NativeSampler> &sampler) = 0;
	};

}