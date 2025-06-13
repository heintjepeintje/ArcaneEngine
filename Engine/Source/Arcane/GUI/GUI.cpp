#include "GUI.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <Arcane/Graphics/Base/Pipeline.hpp>
#include <Arcane/Graphics/Base/Texture.hpp>
#include <Arcane/Graphics/Base/Mesh.hpp>
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

		CompileShader(sContext.GetGraphicsAPI(), "Engine/Shaders/UI/Source/UIShader.vert", "Engine/Shaders/UI/Binaries/Output/UIShader.vert");
		CompileShader(sContext.GetGraphicsAPI(), "Engine/Shaders/UI/Source/UIShader.frag", "Engine/Shaders/UI/Binaries/Output/UIShader.frag");

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

		ShaderBinary vertexShaderBinary = ReadShaderBinary("Engine/Shaders/UI/Binaries/Output/UIShader.vert.spv");
		ShaderBinary fragmentShaderBinary = ReadShaderBinary("Engine/Shaders/UI/Binaries/Output/UIShader.frag.spv");

		PipelineInfo info = PipelineInfo::CreateWithDefaultInfo();
		info.OutputMask = OutputComponent::Color;
		info.CullMode = CullMode::None;
		info.Descriptors = descriptors;
		info.DescriptorCount = 2;
		info.Layout = layout;
		info.VertexShader = vertexShaderBinary;
		info.FragmentShader = fragmentShaderBinary;
		
		sPipeline = Pipeline::Create(sContext, info);
	}

	void BeginFrame() {

	}

	void EndFrame() {

	}

}