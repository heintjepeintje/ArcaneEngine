#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/InputLayout.hpp>
#include <Arcane/Math/Vector.hpp>
#include <Arcane/Native/NativeGraphicsContext.hpp>
#include <Arcane/Math/Math.hpp>

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
		
		void *VertexShaderBinary;
		size_t VertexShaderBinarySize;

		void *FragmentShaderBinary;
		size_t FragmentShaderBinarySize;
	};

	class NativePipeline {
	public:
		static std::shared_ptr<NativePipeline> Create(const std::shared_ptr<NativeGraphicsContext> &context, const PipelineInfo &info);

	public:
		NativePipeline() { }
		virtual ~NativePipeline() { }
	};

}