#include "PBRRenderer.hpp"

#include <Arcane/Graphics/Buffer.hpp>
#include <Arcane/Graphics/Renderer.hpp>
#include <Arcane/Graphics/RenderPass.hpp>
#include <Arcane/Graphics/Pipeline.hpp>
#include <Arcane/Util/FileUtil.hpp>

#define AR_PBR_VERTEX_SHADER_PATH "Engine/Shaders/PBR/Binaries/Output/PBRShader.vert.spv"
#define AR_PBR_FRAGMENT_SHADER_PATH "Engine/Shaders/PBR/Binaries/Output/PBRShader.frag.spv"

#define AR_PBR_SAMPLE_COUNT 16

namespace Arcane {

	static struct {
		Matrix4 Projection;
		Matrix4 View;
		Vector4 Position;
	} sCameraData;

	static struct {
		Matrix4 Model;
		Matrix4 MVP;
		Vector4 Position;
	} sObjectData;

	static struct {
		struct {
			Vector4 Position;
			Color Color;
			float Intensity;
			float _PADDING0[3];
		} PointLights[32];
		uint32_t PointLightCount;
		float _PADDING0[3];
		struct {
			Vector4 Direction;
			Color Color;
		} DirectionalLight;
	} sLightData;

	static Camera3D sCamera;

	static GraphicsContext sContext;

	static Buffer sCameraBuffer;
	static Buffer sObjectBuffer;
	static Buffer sLightBuffer;
	static RendererAPI sRendererAPI;
	static RenderPass sRenderPass;
	static Framebuffer sFramebuffer;
	static Pipeline sPBRPipeline;

	static Sampler sTextureSampler;

	struct RenderSubmit {
		Matrix4 Model;
		Vector3 Position;
		Mesh Mesh;
		Texture AlbedoMap;
		Texture NormalMap;
		Texture MetallicMap;
		Texture RoughnessMap;
		Texture AmbientOcclusionMap;
	};

	static std::vector<RenderSubmit> sRenderSubmissions;

	void PBRRenderer::Init(const GraphicsContext &context) {
		sContext = context;

		sRendererAPI = RendererAPI::Create(sContext);

		const Attachment attachments[] = {
			{ AttachmentType::Color, ImageFormat::RGBA8, AR_PBR_SAMPLE_COUNT },
			{ AttachmentType::DepthStencil, ImageFormat::D24S8, AR_PBR_SAMPLE_COUNT },
		};

		FramebufferInfo framebufferInfo{};
		framebufferInfo.Width = 1920;
		framebufferInfo.Height = 1080;
		framebufferInfo.Attachments = attachments;
		framebufferInfo.AttachmentCount = 2;

		sFramebuffer = Framebuffer::Create(sContext, framebufferInfo);

		Descriptor descriptors[] = {
			{ 0, DescriptorType::UniformBuffer },
			{ 1, DescriptorType::UniformBuffer },
			{ 2, DescriptorType::UniformBuffer },
			
			{ 0, DescriptorType::CombinedImageSampler },
			{ 1, DescriptorType::CombinedImageSampler },
			{ 2, DescriptorType::CombinedImageSampler },
			{ 3, DescriptorType::CombinedImageSampler },
			{ 4, DescriptorType::CombinedImageSampler }
		};

		PipelineInfo pipelineInfo{};
		pipelineInfo.CullMode = CullMode::Back;
		pipelineInfo.WindingOrder = WindingOrder::CounterClockwise;
		pipelineInfo.Topology = PrimitiveTopology::TriangleList;
		pipelineInfo.Descriptors = descriptors;
		pipelineInfo.DescriptorCount = 3;
		pipelineInfo.FillMode = FillMode::Solid;
		pipelineInfo.Layout = {
			{ InputAttribute::Position, 1, InputElementType::Vector3 },
			{ InputAttribute::Normal, 1, InputElementType::Vector3 },
			{ InputAttribute::UV, 1, InputElementType::Vector2 },
			{ InputAttribute::Tangent, 1, InputElementType::Vector3 },
			{ InputAttribute::Bitangent, 1, InputElementType::Vector3 },
		};

		size_t vertexBinarySize = 0, fragmentBinarySize = 0;
		uint8_t *vertexShaderBinary = ReadFileBinary(AR_PBR_VERTEX_SHADER_PATH, &vertexBinarySize);
		uint8_t *fragmentShaderBinary = ReadFileBinary(AR_PBR_FRAGMENT_SHADER_PATH, &fragmentBinarySize);

		pipelineInfo.VertexShaderBinary = vertexShaderBinary;
		pipelineInfo.VertexShaderSize = vertexBinarySize;

		pipelineInfo.FragmentShaderBinary = fragmentShaderBinary;
		pipelineInfo.FragmentShaderSize = fragmentBinarySize;

		pipelineInfo.SampleCount = AR_PBR_SAMPLE_COUNT;
		pipelineInfo.Scissor = { Vector2::Zero(), Vector2::MaxValue() };
		pipelineInfo.Viewport = { Vector2::Zero(), Vector2::MaxValue() };

		sPBRPipeline = Pipeline::Create(sContext, pipelineInfo);

		sRenderPass = RenderPass::Create(sContext, sPBRPipeline, attachments, 2);

		sCameraBuffer = Buffer::Create(sContext, sizeof(sCameraData));
		sPBRPipeline.SetUniformBuffer(0, sCameraBuffer);

		sObjectBuffer = Buffer::Create(sContext, sizeof(sObjectData));
		sPBRPipeline.SetUniformBuffer(1, sObjectBuffer);

		sLightBuffer = Buffer::Create(sContext, sizeof(sLightData));
		sPBRPipeline.SetUniformBuffer(2, sLightBuffer);

		SamplerInfo samplerInfo{};
		samplerInfo.MinFilter = SamplerFilter::Linear; 
		samplerInfo.MagFilter = SamplerFilter::Linear;
		samplerInfo.WrapS = SamplerWrap::Repeat;
		samplerInfo.WrapR = SamplerWrap::Repeat;
		samplerInfo.WrapT = SamplerWrap::Repeat;

		sTextureSampler = Sampler::Create(sContext, samplerInfo);

		std::memset(&sObjectData, 0, sizeof(sObjectData));
		std::memset(&sCameraData, 0, sizeof(sCameraData));
		std::memset(&sLightData, 0, sizeof(sLightData));
	}

	void PBRRenderer::Shutdown() {

	}

	void PBRRenderer::Reload() {
		Descriptor descriptors[] = {
			{ 0, DescriptorType::UniformBuffer },
			{ 1, DescriptorType::UniformBuffer },
			{ 2, DescriptorType::UniformBuffer },
			
			{ 0, DescriptorType::CombinedImageSampler },
			{ 1, DescriptorType::CombinedImageSampler },
			{ 2, DescriptorType::CombinedImageSampler },
			{ 3, DescriptorType::CombinedImageSampler },
			{ 4, DescriptorType::CombinedImageSampler }
		};

		PipelineInfo pipelineInfo{};
		pipelineInfo.CullMode = CullMode::Back;
		pipelineInfo.WindingOrder = WindingOrder::CounterClockwise;
		pipelineInfo.Topology = PrimitiveTopology::TriangleList;
		pipelineInfo.Descriptors = descriptors;
		pipelineInfo.DescriptorCount = 3;
		pipelineInfo.FillMode = FillMode::Solid;
		pipelineInfo.Layout = {
			{ InputAttribute::Position, 1, InputElementType::Vector3 },
			{ InputAttribute::Normal, 1, InputElementType::Vector3 },
			{ InputAttribute::UV, 1, InputElementType::Vector2 },
			{ InputAttribute::Tangent, 1, InputElementType::Vector3 },
			{ InputAttribute::Bitangent, 1, InputElementType::Vector3 },
		};

		size_t vertexBinarySize = 0, fragmentBinarySize = 0;
		uint8_t *vertexShaderBinary = ReadFileBinary(AR_PBR_VERTEX_SHADER_PATH, &vertexBinarySize);
		uint8_t *fragmentShaderBinary = ReadFileBinary(AR_PBR_FRAGMENT_SHADER_PATH, &fragmentBinarySize);

		pipelineInfo.VertexShaderBinary = vertexShaderBinary;
		pipelineInfo.VertexShaderSize = vertexBinarySize;

		pipelineInfo.FragmentShaderBinary = fragmentShaderBinary;
		pipelineInfo.FragmentShaderSize = fragmentBinarySize;

		pipelineInfo.SampleCount = AR_PBR_SAMPLE_COUNT;
		pipelineInfo.Scissor = { Vector2::Zero(), Vector2::MaxValue() };
		pipelineInfo.Viewport = { Vector2::Zero(), Vector2::MaxValue() };

		sPBRPipeline = Pipeline::Create(sContext, pipelineInfo);

		const Attachment attachments[] = {
			{ AttachmentType::Color, ImageFormat::RGBA8, AR_PBR_SAMPLE_COUNT },
			{ AttachmentType::DepthStencil, ImageFormat::D24S8, AR_PBR_SAMPLE_COUNT },
		};

		sRenderPass = RenderPass::Create(sContext, sPBRPipeline, attachments, 2);

		sPBRPipeline.SetUniformBuffer(0, sCameraBuffer);
		sPBRPipeline.SetUniformBuffer(1, sObjectBuffer);
		sPBRPipeline.SetUniformBuffer(2, sLightBuffer);

		std::memset(&sObjectData, 0, sizeof(sObjectData));
		std::memset(&sCameraData, 0, sizeof(sCameraData));
		std::memset(&sLightData, 0, sizeof(sLightData));
	}

	void PBRRenderer::Begin(const Camera3D &camera) {
		sCamera = camera;

		sCameraData.Projection = Matrix4::Transpose(camera.GetProjectionMatrix());
		sCameraData.View = Matrix4::Transpose(camera.GetViewMatrix());
		sCameraData.Position = Vector4(camera.Position, 1.0);

		sCameraBuffer.SetData((const void*)&sCameraData);
	}

	void PBRRenderer::AddLight(const Vector3 &position, const PointLight &light) {
		uint32_t index = sLightData.PointLightCount++;
		sLightData.PointLights[index].Color = light.Color;
		sLightData.PointLights[index].Intensity = light.Intensity;
		sLightData.PointLights[index].Position = Vector4(position, 1.0);
	}

	void PBRRenderer::AddLight(const Vector3 &direction, const DirectionalLight &light) {
		sLightData.DirectionalLight.Color = light.Color;
		sLightData.DirectionalLight.Direction = Vector4(Vector3::Normalize(direction), 1.0);
	}

	void PBRRenderer::Submit(const Transform &transform, const Mesh &mesh, const PBRMaterial &material) {
		sRenderSubmissions.emplace_back(
			transform.GetModelMatrix(),
			transform.Position,
			mesh,
			material.AlbedoMap,
			material.NormalMap,
			material.MetallicMap,
			material.RoughnessMap,
			material.AmbientOcclusionMap
		);
	}

	void PBRRenderer::End() {
		sRendererAPI.Begin();
		sRendererAPI.BeginRenderPass(sRenderPass, sFramebuffer);
		sRendererAPI.Clear();
		sRendererAPI.SetViewport(sContext.GetWindow().GetClientSize());
		sRendererAPI.SetScissor(sContext.GetWindow().GetClientSize());
		sFramebuffer.Resize(sContext.GetWindow().GetClientSize());
		sRendererAPI.SetClearColor(Color::Black());

		sLightBuffer.SetData(&sLightData);
		
		for (const RenderSubmit &submission : sRenderSubmissions) {
			sObjectData.Model = Matrix4::Transpose(submission.Model);
			sObjectData.MVP = Matrix4::Transpose(sCamera.GetProjectionMatrix() * sCamera.GetViewMatrix() * submission.Model);
			sObjectData.Position = Vector4(submission.Position, 1.0);

			sObjectBuffer.SetData((const void*)&sObjectData);
			sPBRPipeline.SetUniformBuffer(1, sObjectBuffer);

			sPBRPipeline.SetCombinedImageSampler(0, submission.AlbedoMap, sTextureSampler);
			sPBRPipeline.SetCombinedImageSampler(1, submission.NormalMap, sTextureSampler);
			sPBRPipeline.SetCombinedImageSampler(2, submission.MetallicMap, sTextureSampler);
			sPBRPipeline.SetCombinedImageSampler(3, submission.RoughnessMap, sTextureSampler);
			sPBRPipeline.SetCombinedImageSampler(4, submission.AmbientOcclusionMap, sTextureSampler);

			sRendererAPI.SetMesh(submission.Mesh);
			sRendererAPI.DrawIndexed(1, submission.Mesh.GetIndexCount());
		}
		sRendererAPI.EndRenderPass();
		sRendererAPI.End();

		sRenderSubmissions.clear();

		std::memset(&sLightData, 0, sizeof(sLightData));
		std::memset(&sObjectData, 0, sizeof(sObjectData));
		std::memset(&sCameraData, 0, sizeof(sCameraData));
	}

}