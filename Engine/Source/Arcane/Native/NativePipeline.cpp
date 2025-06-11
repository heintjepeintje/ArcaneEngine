#include "NativePipeline.hpp"

namespace Arcane {

	PipelineInfo PipelineInfo::CreateWithDefaultInfo() {
		PipelineInfo defaultInfo{};
		defaultInfo.OutputMask = OutputComponent::All;
		defaultInfo.CullMode = CullMode::Back;
		defaultInfo.FillMode = FillMode::Solid;
		defaultInfo.OutputMask = OutputComponent::All;
		defaultInfo.Scissor = Rect2D(Vector2::Zero(), Vector2::MaxValue());
		defaultInfo.Viewport = Rect2D(Vector2::Zero(), Vector2::MaxValue());
		defaultInfo.Topology = PrimitiveTopology::TriangleList;
		defaultInfo.PrimitiveRestart = false;
		defaultInfo.WindingOrder = WindingOrder::CounterClockwise;
		defaultInfo.PointSize = 1;
		defaultInfo.LineWidth = 1;
		defaultInfo.ElementSize = sizeof(uint32_t);
		defaultInfo.PolygonOffsetFactor = 0.0f;
		defaultInfo.PolygonOffsetUnits = 0.0f;

		return defaultInfo;
	}

}