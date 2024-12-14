#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/InputLayout.hpp>
#include <Arcane/Math/Vector.hpp>

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

	struct PipelineInfo {
		CullMode CullMode;
		FillMode FillMode;
		PrimitiveTopology Topology;	

		InputLayout Layout;
		
		void *VertexShaderBinary;
		size_t VertexShaderBinarySize;

		void *FragmentShaderBinary;
		size_t FragmentShaderBinarySize;
	};

}