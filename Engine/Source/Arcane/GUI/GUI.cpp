#include "GUI.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <Arcane/Graphics/Pipeline.hpp>
#include <Arcane/Graphics/Texture.hpp>
#include <Arcane/Graphics/Mesh.hpp>
#include <Arcane/Util/FileUtil.hpp>

namespace Arcane::GUI {

	static ImGuiIO *sIO;
	static Window sWindow;
	static GraphicsContext sContext;

	static Pipeline sPipeline;
	static Buffer sVertexBuffer;
	static Buffer sIndexBuffer;
	static Mesh sMesh;

	void Init(const Window &window, const GraphicsContext &context) {
		sWindow = window;
		sContext = context;

		ImGui::CreateContext();
		sIO = &ImGui::GetIO();

		sIO->BackendPlatformName = "Arcane";
		sIO->BackendRendererName = "Arcane GUI Renderer";

		InputLayout layout = {
			{ InputAttribute::Position, 1, InputElementType::Vector2f32, false },
			{ InputAttribute::UV, 1, InputElementType::Vector2f32, false },
			{ InputAttribute::Color, 1, InputElementType::Vector4u8, true }
		};

		sVertexBuffer = Buffer::Create(sContext, sizeof(ImDrawVert) * 1000);
		sIndexBuffer = Buffer::Create(sContext, sizeof(ImDrawIdx) * 2000);

		sMesh = Mesh::Create(sContext);
		sMesh.SetVertexBuffer(0, layout, sVertexBuffer);
		sMesh.SetIndexBuffer(sIndexBuffer);

		Descriptor descriptors[] = {
			{ 1, DescriptorType::UniformBuffer },
			{ 0, DescriptorType::CombinedImageSampler }
		};

		size_t vertexBinarySize, fragmentBinarySize;
		uint8_t *vertexBinary, *fragmentBinary;
		vertexBinary = ReadFileBinary("Engine/Shaders/UI/Binaries/Output/UIShader.vert", &vertexBinarySize);
		fragmentBinary = ReadFileBinary("Engine/Shaders/UI/Binaries/Output/UIShader.frag", &fragmentBinarySize);

		PipelineInfo info = PipelineInfo::CreateWithDefaultInfo();
		info.OutputMask = OutputComponent::Color;
		info.CullMode = CullMode::None;
		info.Descriptors = descriptors;
		info.DescriptorCount = 2;
		info.Layout = layout;
		info.VertexShaderBinary = vertexBinary;
		info.VertexShaderSize = vertexBinarySize;
		info.FragmentShaderBinary = fragmentBinary;
		info.FragmentShaderSize = fragmentBinarySize;
		
		sPipeline = Pipeline::Create(sContext, info);
	}

	void BeginFrame() {

	}

	void EndFrame() {

	}

}