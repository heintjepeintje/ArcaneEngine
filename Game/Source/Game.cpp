#include <Arcane/System/Window.hpp>
#include <Arcane/Graphics/GraphicsContext.hpp>
#include <Arcane/Graphics/InputLayout.hpp>
#include <Arcane/Graphics/Buffer.hpp>
#include <Arcane/Graphics/Pipeline.hpp>

#include <iostream>

using namespace Arcane;

int main(int argc, char **argv) {
	Window window = Window::Create(1920 / 2, 1080 / 2, "Arcane Engine");
	window.SetVisible(true);
	window.SetMaximized(true);

	GraphicsContext context = GraphicsContext::Create(window);

	std::cout << context.GetVersionMajor() << "." << context.GetVersionMinor() << "." << context.GetPatchLevel() << std::endl;

	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		 0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
	};

	int32_t indices[] = {
		0, 1, 2
	};

	Buffer vertexBuffer = Buffer::Create(context, BufferType::Vertex, sizeof(vertices));
	void *mapped = vertexBuffer.Map(MapMode::Write);
	memcpy(mapped, vertices, sizeof(vertices));
	vertexBuffer.Unmap();

	Buffer indexBuffer = Buffer::Create(context, BufferType::Index, sizeof(indices));
	mapped = indexBuffer.Map(MapMode::Write);
	memcpy(mapped, indices, sizeof(indices));
	indexBuffer.Unmap();

	InputLayout layout = {
		{ InputAttribute::Position, 1, InputElementType::Vector3 },
		{ InputAttribute::Normal, 1, InputElementType::Vector3 },
		{ InputAttribute::UV, 1, InputElementType::Vector2 },
		{ InputAttribute::Color, 1, InputElementType::Vector4 }
	};

	PipelineInfo pipelineInfo = { };
	pipelineInfo.CullMode = CullMode::Back;
	pipelineInfo.WindingOrder = WindingOrder::CounterClockwise;
	pipelineInfo.FillMode = FillMode::Solid;
	pipelineInfo.Topology = PrimitiveTopology::TriangleList;

	while (!window.IsClosed()) {
		context.Present();
		window.Update();
	}

	window.Destroy();

	return 0;
}