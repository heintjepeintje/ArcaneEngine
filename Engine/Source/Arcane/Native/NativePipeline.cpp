#include "NativePipeline.hpp"

namespace Arcane {

	PipelineInfo PipelineInfo::CreateWithDefaultInfo() {
		PipelineInfo defaultInfo{};
		defaultInfo.CullMode = CullMode::Back;
		defaultInfo.FillMode = FillMode::Solid;
		defaultInfo.OutputMask = OutputComponent::All;
		defaultInfo.Scissor = Rect2D(Vector2::Zero(), Vector2::MaxValue());
		defaultInfo.Viewport = Rect2D(Vector2::Zero(), Vector2::MaxValue());
		defaultInfo.Topology = PrimitiveTopology::TriangleList;
		defaultInfo.WindingOrder = WindingOrder::CounterClockwise;
		defaultInfo.PointSize = 10;
		defaultInfo.LineWidth = 10;

		return defaultInfo;
	}

}