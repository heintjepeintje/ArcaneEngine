#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/InputLayout.hpp>
#include <Arcane/Math/Vector.hpp>
#include <Arcane/Math/Rect2D.hpp>
#include "NativeGraphicsContext.hpp"
#include "NativeBuffer.hpp"

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
		Sampler2D
	};

	struct Descriptor {
		uint32_t Binding;
		uint32_t Count;
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
		
		char *VertexShaderSource;
		size_t VertexShaderSourceLength;

		char *FragmentShaderSource;
		size_t FragmentShaderSourceLength;

		Descriptor *Descriptors;
		uint32_t DescriptorCount;
	};

	class NativePipeline {
	public:
		static std::shared_ptr<NativePipeline> Create(const std::shared_ptr<NativeGraphicsContext> &context, const PipelineInfo &info);

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

		virtual void SetDescriptor(uint32_t binding, const std::shared_ptr<NativeBuffer> &uniformBuffer) = 0;
	};

}