#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/InputLayout.hpp>
#include <Arcane/Math/Vector.hpp>
#include <Arcane/Native/NativeGraphicsContext.hpp>

namespace Arcane {

	enum class CullMode {
		None,
		Front,
		Back
	};

	enum class WindingOrder {
		Clockwise,
		CounterClockwise
	};

	enum class FillMode {
		Solid,
		Wireframe
	};

	enum class PrimitiveTopology {
		TriangleList,
		TriangleStrip,
		LineList,
		LineStrip,
		PointList
	};

	struct Viewport {
		Vector2 Offset;
		Vector2 Size;
	};

	struct Scissor {
		Vector2 Offset;
		Vector2 Size;
	};

	struct PipelineInfo {
		PipelineInfo() { }
		~PipelineInfo() { }

		CullMode CullMode;
		WindingOrder WindingOrder;
		FillMode FillMode;
		PrimitiveTopology Topology;	

		InputLayout Layout;
		
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