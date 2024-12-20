#include <Arcane/Arcane.hpp>

#include <fstream>
#include <iostream>
#include <cstring>

using namespace Arcane;

int main(int argc, char **argv) {
	Window window = Window::Create(1920 / 2, 1080 / 2, "Arcane Engine");
	window.SetVisible(true);
	window.SetMaximized(true);

	GraphicsContext context = GraphicsContext::Create(window);

	std::cout << "Using OpenGL " << context.GetVersionMajor() << "." << context.GetVersionMinor() << "." << context.GetPatchLevel() << std::endl;

	MeshData data = LoadCube(Vector3(2.0f));

	Buffer vertexBuffer = Buffer::Create(context, BufferType::Vertex, data.Vertices.size() * sizeof(Vertex));
	void *mapped = vertexBuffer.Map(MapMode::Write);
	memcpy(mapped, data.Vertices.data(), vertexBuffer.GetSize());
	vertexBuffer.Unmap();

	Buffer indexBuffer = Buffer::Create(context, BufferType::Index, data.Indices.size() * sizeof(uint32_t));
	mapped = indexBuffer.Map(MapMode::Write);
	memcpy(mapped, data.Indices.data(), indexBuffer.GetSize());
	indexBuffer.Unmap();

	for (uint32_t i = 0; i < data.Indices.size(); i++) {
		std::printf("%u: { %f, %f, %f }, { %f, %f, %f }\n", i, data.Vertices[data.Indices[i]].Position.x, data.Vertices[data.Indices[i]].Position.y, data.Vertices[data.Indices[i]].Position.z, data.Vertices[data.Indices[i]].Normal.x, data.Vertices[data.Indices[i]].Normal.y, data.Vertices[data.Indices[i]].Normal.z);
	}

	InputLayout layout = {
		{ InputAttribute::Position, 1, InputElementType::Vector3 },
		{ InputAttribute::Normal, 1, InputElementType::Vector3 },
		{ InputAttribute::UV, 1, InputElementType::Vector2 },
		{ InputAttribute::Color, 1, InputElementType::Vector4 }
	};

	Mesh mesh = Mesh::Create(context);
	mesh.SetVertexBuffer(0, layout, vertexBuffer);
	mesh.SetIndexBuffer(indexBuffer);

	FILE *file = fopen("Engine/Shaders/BasicShader/Source/BasicShader.vert", "r");
	fseek(file, 0, SEEK_END);
	uint64_t vertexSourceLength = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *vertexSource = (char*)alloca((vertexSourceLength + 1) * sizeof(char));
	memset(vertexSource, 0, vertexSourceLength + 1);
	fread((void*)vertexSource, 1, vertexSourceLength, file);
	std::printf("Vertex Shader:\n%s\n", vertexSource);

	fclose(file);

	file = fopen("Engine/Shaders/BasicShader/Source/BasicShader.frag", "r");
	fseek(file, 0, SEEK_END);
	uint64_t fragmentSourceLength = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *fragmentSource = (char*)alloca((fragmentSourceLength + 1) * sizeof(char));
	memset(fragmentSource, 0, fragmentSourceLength + 1);
	fread((void*)fragmentSource, 1, fragmentSourceLength, file);
	std::printf("Fragment Shader:\n%s\n", fragmentSource);

	fclose(file);

	Descriptor descriptors[] = {
		{ 0, 1, DescriptorType::UniformBuffer }
	};

	Matrix4 convert = Matrix4::Identity(Vector4(1.0f, 1.0f, -1.0f, 1.0f));
	Matrix4 projection = Matrix4::Perspective(ToRadians(90.0f), window.GetClientSize(), 0.1f, 1000.0f);
	Matrix4 view = Matrix4::LookAt(Vector3(0.0f, 0.0f, -10.0f), Vector3(0, 0, 0), Vector3(0, 1, 0));

	Buffer cameraBuffer = Buffer::Create(context, BufferType::Uniform, 3 * sizeof(Matrix4));
	Matrix4 *matrices = (Matrix4*)cameraBuffer.Map(MapMode::Write);
	matrices[0] = Matrix4::Transpose(convert);
	matrices[1] = Matrix4::Transpose(projection);
	matrices[2] = Matrix4::Transpose(view);
	cameraBuffer.Unmap();

	PipelineInfo pipelineInfo = { };
	pipelineInfo.CullMode = CullMode::None;
	pipelineInfo.WindingOrder = WindingOrder::CounterClockwise;
	pipelineInfo.FillMode = FillMode::Solid;
	pipelineInfo.Topology = PrimitiveTopology::TriangleList;
	pipelineInfo.Layout = layout;
	pipelineInfo.Viewport = { Vector2::Zero(), Vector2::MaxValue() };
	pipelineInfo.Scissor = { Vector2::Zero(), Vector2::MaxValue() };
	pipelineInfo.VertexShaderSource = vertexSource;
	pipelineInfo.VertexShaderSourceLength = vertexSourceLength;
	pipelineInfo.FragmentShaderSource = fragmentSource;
	pipelineInfo.FragmentShaderSourceLength = fragmentSourceLength;
	pipelineInfo.Descriptors = descriptors;
	pipelineInfo.DescriptorCount = 1;

	Pipeline pipeline = Pipeline::Create(context, pipelineInfo);

	pipeline.SetDescriptor(0, cameraBuffer);

	RendererAPI renderer = RendererAPI::Create(context);
	renderer.SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!window.IsClosed()) {
		renderer.Clear();
		
		renderer.SetViewport(Rect2D(window.GetClientSize()));
		renderer.SetScissor(Rect2D(window.GetClientSize()));
		renderer.SetPipeline(pipeline);
		renderer.SetMesh(mesh);
		renderer.DrawIndexed(36);

		context.Present();
		window.Update();
	}

	window.Destroy();

	return 0;
}