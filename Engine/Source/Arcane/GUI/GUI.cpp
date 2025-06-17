#include "GUI.hpp"

#include <imgui_internal.h>

#include <Arcane/Graphics/Base/Pipeline.hpp>
#include <Arcane/Graphics/Base/RenderPass.hpp>
#include <Arcane/Graphics/Base/Framebuffer.hpp>
#include <Arcane/Graphics/Base/Texture.hpp>
#include <Arcane/Graphics/Base/Mesh.hpp>
#include <Arcane/Util/FileUtil.hpp>
#include <Arcane/Math/Matrix4.hpp>

#include <Arcane/Graphics/Base/RendererAPI.hpp>

#include <Arcane/System/Memory.hpp>

namespace Arcane::GUI {

	static ImGuiIO *sIO;
	static Window sWindow;
	static GraphicsContext sContext;
	static RendererAPI sRendererAPI;
	
	static std::vector<Texture> sTextures;
	static Sampler sSampler;

	static Pipeline sPipeline;
	static Framebuffer sFramebuffer;
	static RenderPass sRenderPass;
	static Buffer sVertexBuffer;
	static Buffer sIndexBuffer;
	static Buffer sUniformBuffer;
	static Mesh sMesh;

	void Init(const Window &window, const GraphicsContext &context, const RendererAPI &rendererAPI) {
		sWindow = window;
		sContext = context;
		sRendererAPI = rendererAPI;

		CompileShader(sContext.GetGraphicsAPI(), "Engine/Shaders/UI/Source/UIShader.vert", "Engine/Shaders/UI/Binaries/Output/UIShader.vert.spv");
		CompileShader(sContext.GetGraphicsAPI(), "Engine/Shaders/UI/Source/UIShader.frag", "Engine/Shaders/UI/Binaries/Output/UIShader.frag.spv");

		ImGui::CreateContext();
		sIO = &ImGui::GetIO();

		sIO->BackendPlatformName = "Arcane";
		sIO->BackendRendererName = "Arcane GUI Renderer";

		SamplerInfo samplerInfo{};
		samplerInfo.MinFilter = SamplerFilter::Linear;
		samplerInfo.MagFilter = SamplerFilter::Linear;
		samplerInfo.MipmapFilter = SamplerFilter::Linear;
		samplerInfo.MinLOD = 0.0f;
		samplerInfo.MaxLOD = 0.0f;
		samplerInfo.BorderColor = Color::White();
		samplerInfo.WrapS = SamplerWrap::ClampToEdge;
		samplerInfo.WrapT = SamplerWrap::ClampToEdge;
		samplerInfo.WrapR = SamplerWrap::ClampToEdge;

		sSampler = Sampler::Create(sContext, samplerInfo);

		const InputLayout inputLayout = {
			{ InputAttribute::Position, 1, InputElementType::Vector2f32, false },
			{ InputAttribute::UV, 1, InputElementType::Vector2f32, false },
			{ InputAttribute::Color, 1, InputElementType::Vector4f32, false }
		};

		sUniformBuffer = Buffer::Create(sContext, sizeof(Matrix4), BufferFlag::Static);
		sVertexBuffer = Buffer::Create(sContext, 1024 * sizeof(ImDrawVert), BufferFlag::Mutable);
		sIndexBuffer = Buffer::Create(sContext, 1024 * sizeof(ImDrawIdx), BufferFlag::Mutable);

		sMesh = Mesh::Create(sContext);
		sMesh.SetVertexBuffer(0, inputLayout, sVertexBuffer);
		sMesh.SetIndexBuffer(sIndexBuffer);
		
		unsigned char *pixels;
		int width, height;
		sIO->Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		BufferRef fontBuffer = AllocateBuffer(width * height * sizeof(uint32_t));
		CopyMemory(fontBuffer.GetPointer(), pixels, fontBuffer.GetSize());

		ImageData imageData{};
		imageData.Width = static_cast<uint32_t>(width);
		imageData.Height = static_cast<uint32_t>(height);
		imageData.Depth = 1;
		imageData.Format = ImageFormat::RGBA8U;
		imageData.Data = fontBuffer;

		TextureInfo textureInfo{};
		textureInfo.Type = TextureType::Texture2D;
		textureInfo.Width = imageData.Width;
		textureInfo.Height = imageData.Height;
		textureInfo.Depth = imageData.Depth;
		textureInfo.Format = imageData.Format;
		textureInfo.Levels = 1;
		textureInfo.Layers = 1;
		textureInfo.Samples = 1;
		textureInfo.FixedSampleLocations = true;

		Texture t = Texture::Create(sContext, textureInfo);
		ImTextureID fontTextureID = CreateTextureID(t);
		sIO->Fonts->SetTexID(fontTextureID);

		const ImageFormat attachments[] = {
			ImageFormat::RGBA8U,
		};

		FramebufferInfo framebufferInfo{};
		framebufferInfo.Width = 1920;
		framebufferInfo.Height = 1080;
		framebufferInfo.Attachments = attachments;
		framebufferInfo.AttachmentCount = 1;
		framebufferInfo.Samples = 1;
		framebufferInfo.FixedSampleLocations = true;

		sFramebuffer = Framebuffer::Create(sContext, framebufferInfo);

		Descriptor descriptors[] = {
			{ 0, DescriptorType::UniformBuffer },
			{ 0, DescriptorType::CombinedImageSampler }
		};
		
		BufferRef vertexShaderBinary = ReadFileBinary("Engine/Shaders/UI/Binaries/Output/UIShader.vert.spv");
		BufferRef fragmentShaderBinary = ReadFileBinary("Engine/Shaders/UI/Binaries/Output/UIShader.frag.spv");
		
		PipelineInfo pipelineInfo = PipelineInfo::CreateWithDefaultInfo();
		pipelineInfo.CullMode = CullMode::None;
		pipelineInfo.Descriptors = descriptors;
		pipelineInfo.DescriptorCount = 2;
		pipelineInfo.Layout = inputLayout;
		pipelineInfo.VertexShaderBinary = vertexShaderBinary;
		pipelineInfo.FragmentShaderBinary = fragmentShaderBinary;
		pipelineInfo.SampleCount = 1;
		pipelineInfo.PrimitiveRestart = false;
		pipelineInfo.ElementSize = sizeof(ImDrawIdx);

		sPipeline = Pipeline::Create(sContext, pipelineInfo);
		sPipeline.SetUniformBuffer(0, sUniformBuffer);

		sRenderPass = RenderPass::Create(sContext, sPipeline, attachments, 1);
	}

	void BeginFrame() {
		Vector2 displaySize = sWindow.GetClientSize();
		sIO->DisplaySize = ImVec2(displaySize.X, displaySize.Y);
		ImGui::NewFrame();
	}

	void EndFrame() {
		ImGui::Render();

		ImDrawData *drawData = ImGui::GetDrawData();

		Matrix4 projection = Matrix4::Transpose(Matrix4::OrthoRH_ZO(
			drawData->DisplayPos.x, drawData->DisplayPos.x + drawData->DisplaySize.x,
			drawData->DisplayPos.y, drawData->DisplayPos.y + drawData->DisplaySize.y,
			-1.0f, 1.0f
		));

		sUniformBuffer.SetData(sizeof(Matrix4), projection.Data);
	}

	void Draw() {
		ImDrawData *drawData = ImGui::GetDrawData();

		Vector2 viewportSize = Vector2(
			drawData->DisplaySize.x * drawData->FramebufferScale.x,
			drawData->DisplaySize.y * drawData->FramebufferScale.y
		);

		sFramebuffer.Resize(viewportSize);

		sRendererAPI.BeginRenderPass(sRenderPass, sFramebuffer);
		sRendererAPI.SetViewport(Rect2D(viewportSize));
		sRendererAPI.Clear();
		
		for (int i = 0; i < drawData->CmdListsCount; i++) {
			ImDrawList *drawList = drawData->CmdLists[i];
			const ImDrawIdx *indices = drawList->IdxBuffer.Data;
			const ImDrawVert *vertices = drawList->VtxBuffer.Data;

			if (sVertexBuffer.GetSize() < drawList->VtxBuffer.Size * sizeof(ImDrawVert)) {
				sVertexBuffer.Resize(drawList->VtxBuffer.Size * sizeof(ImDrawVert));
			}
			sVertexBuffer.SetData(0, drawList->VtxBuffer.Size * sizeof(ImDrawVert), vertices);
			
			if (sIndexBuffer.GetSize() < drawList->IdxBuffer.Size * sizeof(ImDrawIdx)) {
				sIndexBuffer.Resize(drawList->IdxBuffer.Size * sizeof(ImDrawIdx));
			}
			sIndexBuffer.SetData(0, drawList->IdxBuffer.Size * sizeof(ImDrawIdx), indices);

			for (int j = 0; j < drawList->CmdBuffer.Size; j++) {
				const ImDrawCmd &cmd = drawList->CmdBuffer[j];

				if (cmd.UserCallback) {
					cmd.UserCallback(drawList, &cmd);
				} else {
					Texture &texture = GetTextureFromID(cmd.TextureId);
					sPipeline.SetCombinedImageSampler(0, texture, sSampler);

					sRendererAPI.SetMesh(sMesh);
					sRendererAPI.DrawIndexed(1, cmd.ElemCount, cmd.IdxOffset, cmd.VtxOffset);
				}
			}
		}

		sRendererAPI.EndRenderPass();
	}

	Framebuffer &GetOutputFramebuffer() {
		return sFramebuffer;
	}

	ImTextureID CreateTextureID(const Texture &texture) {
		sTextures.push_back(texture);
		return reinterpret_cast<ImTextureID>(sTextures.size() - 1);
	}

	Texture &GetTextureFromID(ImTextureID id) {
		return sTextures[static_cast<size_t>(id)];
	}

}