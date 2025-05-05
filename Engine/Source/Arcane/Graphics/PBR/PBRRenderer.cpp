#include "PBRRenderer.hpp"

#include <Arcane/Graphics/Buffer.hpp>
#include <Arcane/Graphics/Renderer.hpp>
#include <Arcane/Graphics/RenderPass.hpp>
#include <Arcane/Graphics/Pipeline.hpp>
#include <Arcane/Util/FileUtil.hpp>
#include <Arcane/System/Time.hpp>

#define AR_VERT_SHADER_PATH(folder, name) "Engine/Shaders/"#folder"/Binaries/Output/"#name".vert.spv"
#define AR_FRAG_SHADER_PATH(folder, name) "Engine/Shaders/"#folder"/Binaries/Output/"#name".frag.spv"

#define AR_GEOMETRY_VERTEX_SHADER_PATH 			AR_VERT_SHADER_PATH(Geometry, 		GeometryShader)
#define AR_GEOMETRY_FRAGMENT_SHADER_PATH 		AR_FRAG_SHADER_PATH(Geometry, 		GeometryShader)
#define AR_LIGHT_VERTEX_SHADER_PATH 			AR_VERT_SHADER_PATH(Light, 			LightShader)
#define AR_LIGHT_FRAGMENT_SHADER_PATH 			AR_FRAG_SHADER_PATH(Light, 			LightShader)
#define AR_POST_PROCESS_VERTEX_SHADER_PATH 		AR_VERT_SHADER_PATH(PostProcess, 	PostProcess)
#define AR_POST_PROCESS_FRAGMENT_SHADER_PATH 	AR_FRAG_SHADER_PATH(PostProcess, 	PostProcess)
#define AR_SHADOW_VERTEX_SHADER_PATH			AR_VERT_SHADER_PATH(Shadow, 		Shadow)
#define AR_SHADOW_FRAGMENT_SHADER_PATH			AR_FRAG_SHADER_PATH(Shadow, 		Shadow)

#define AR_PBR_SAMPLE_COUNT 1

#define AR_PBR_SHADOW_MAP_WIDTH 1024
#define AR_PBR_SHADOW_MAP_HEIGHT 1024

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

	static struct {
		float Gamma;
		float Exposure;
	} sPostProcessSettingsData;

	static struct {
		Matrix4 LightProjection;
		Matrix4 LightView;
	} sShadowPassData;

	static Camera3D sCamera;

	static GraphicsContext sContext;

	static Buffer sCameraBuffer;
	static Buffer sObjectBuffer;
	static Buffer sLightBuffer;
	static RendererAPI sRendererAPI;

	static RenderPass sGeometryRenderPass;
	static Framebuffer sGeometryFramebuffer;
	static Pipeline sGeometryPipeline;

	static RenderPass sLightRenderPass;
	static Framebuffer sLightFramebuffer;
	static Pipeline sLightPipeline;
	
	static RenderPass sPostProcessRenderPass;
	static Framebuffer sPostProcessFramebuffer;
	static Pipeline sPostProcessPipeline;
	static Buffer sPostProcessSettingsBuffer;

	static RenderPass sShadowPass;
	static Pipeline sShadowPipeline;
	static Framebuffer sShadowMap;
	static Buffer sShadowPassBuffer;

	static Color sClearColor;
	
	static Mesh sQuadMesh;
	static Sampler sDefaultSampler;

	struct RenderSubmission {
		Matrix4 Model;
		Vector3 Position;
		Mesh Mesh;
		Texture AlbedoMap;
		Texture NormalMap;
		Texture MetallicMap;
		Texture RoughnessMap;
		Texture AmbientOcclusionMap;
	};

	static std::vector<RenderSubmission> sRenderSubmissions;

	void PBRRenderer::Init(const GraphicsContext &context) {
		AR_PROFILE_FUNCTION();
		sContext = context;

		sRendererAPI = RendererAPI::Create(sContext);
		sCameraBuffer = Buffer::Create(sContext, sizeof(sCameraData));
		std::memset(&sCameraData, 0, sizeof(sCameraData));
		sObjectBuffer = Buffer::Create(sContext, sizeof(sObjectData));
		std::memset(&sObjectData, 0, sizeof(sObjectData));
		sShadowPassBuffer = Buffer::Create(sContext, sizeof(sShadowPassData));
		std::memset(&sShadowPassData, 0, sizeof(sShadowPassData));
		sLightBuffer = Buffer::Create(sContext, sizeof(sLightData));
		std::memset(&sLightData, 0, sizeof(sLightData));
		sPostProcessSettingsBuffer = Buffer::Create(sContext, sizeof(sPostProcessSettingsData));
		std::memset(&sPostProcessSettingsData, 0, sizeof(sPostProcessSettingsData));

		InputLayout quadLayout = {
			{ InputAttribute::Position, 1, InputElementType::Vector2f32 },
			{ InputAttribute::UV, 1, InputElementType::Vector2f32 },
		};

		Buffer quadVertexBuffer = Buffer::Create(sContext, 16 * sizeof(float));
		Buffer quadIndexBuffer = Buffer::Create(sContext, 6 * sizeof(uint32_t));

		float quadVertices[] = {
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f 
		};
		quadVertexBuffer.SetData((const void*)quadVertices);

		uint32_t quadIndices[] = {
			0, 1, 2,
			0, 2, 3
		};
		quadIndexBuffer.SetData((const void*)quadIndices);

		sQuadMesh = Mesh::Create(sContext);
		sQuadMesh.SetVertexBuffer(0, quadLayout, quadVertexBuffer);
		sQuadMesh.SetIndexBuffer(quadIndexBuffer);

		SamplerInfo samplerInfo{};
		samplerInfo.MinFilter = SamplerFilter::Linear; 
		samplerInfo.MagFilter = SamplerFilter::Linear;
		samplerInfo.MipmapFilter = SamplerFilter::Linear;
		samplerInfo.WrapS = SamplerWrap::Repeat;
		samplerInfo.WrapR = SamplerWrap::Repeat;
		samplerInfo.WrapT = SamplerWrap::Repeat;

		sDefaultSampler = Sampler::Create(sContext, samplerInfo);

		const Attachment geometryAttachments[] = {
			{ AttachmentType::Color, ImageFormat::RGB32F, AR_PBR_SAMPLE_COUNT },
			{ AttachmentType::Color, ImageFormat::RGB32F, AR_PBR_SAMPLE_COUNT },
			{ AttachmentType::Color, ImageFormat::RGB32F, AR_PBR_SAMPLE_COUNT },
			{ AttachmentType::Color, ImageFormat::RGB32F, AR_PBR_SAMPLE_COUNT },
			{ AttachmentType::Color, ImageFormat::RGB32F, AR_PBR_SAMPLE_COUNT },
			{ AttachmentType::DepthStencil, ImageFormat::D24S8, AR_PBR_SAMPLE_COUNT },
		};

		FramebufferInfo geometryFramebufferInfo{};
		geometryFramebufferInfo.Width = 1920;
		geometryFramebufferInfo.Height = 1080;
		geometryFramebufferInfo.Attachments = geometryAttachments;
		geometryFramebufferInfo.AttachmentCount = 6;

		sGeometryFramebuffer = Framebuffer::Create(sContext, geometryFramebufferInfo);

		Descriptor geometryDescriptors[] = {
			{ 0, DescriptorType::UniformBuffer },
			{ 1, DescriptorType::UniformBuffer },
			{ 2, DescriptorType::UniformBuffer },
			
			{ 0, DescriptorType::CombinedImageSampler },
			{ 1, DescriptorType::CombinedImageSampler },
			{ 2, DescriptorType::CombinedImageSampler },
			{ 3, DescriptorType::CombinedImageSampler },
			{ 4, DescriptorType::CombinedImageSampler }
		};
		
		InputLayout geometryInputLayout = {
			{ InputAttribute::Position, 1, InputElementType::Vector3f32 },
			{ InputAttribute::Normal, 1, InputElementType::Vector3f32 },
			{ InputAttribute::UV, 1, InputElementType::Vector2f32 },
			{ InputAttribute::Tangent, 1, InputElementType::Vector3f32 },
			{ InputAttribute::Bitangent, 1, InputElementType::Vector3f32 },
		};
		
		size_t vertexBinarySize = 0, fragmentBinarySize = 0;
		uint8_t *vertexShaderBinary = ReadFileBinary(AR_GEOMETRY_VERTEX_SHADER_PATH, &vertexBinarySize);
		uint8_t *fragmentShaderBinary = ReadFileBinary(AR_GEOMETRY_FRAGMENT_SHADER_PATH, &fragmentBinarySize);
		
		PipelineInfo geometryPipelineInfo = PipelineInfo::CreateWithDefaultInfo();
		geometryPipelineInfo.Descriptors = geometryDescriptors;
		geometryPipelineInfo.DescriptorCount = 7;
		geometryPipelineInfo.Layout = geometryInputLayout;
		geometryPipelineInfo.VertexShaderBinary = vertexShaderBinary;
		geometryPipelineInfo.VertexShaderSize = vertexBinarySize;
		geometryPipelineInfo.FragmentShaderBinary = fragmentShaderBinary;
		geometryPipelineInfo.FragmentShaderSize = fragmentBinarySize;
		geometryPipelineInfo.SampleCount = AR_PBR_SAMPLE_COUNT;

		sGeometryPipeline = Pipeline::Create(sContext, geometryPipelineInfo);
		sGeometryPipeline.SetUniformBuffer(0, sCameraBuffer);
		sGeometryPipeline.SetUniformBuffer(1, sObjectBuffer);
		sGeometryPipeline.SetUniformBuffer(2, sShadowPassBuffer);

		free(vertexShaderBinary);
		free(fragmentShaderBinary);

		sGeometryRenderPass = RenderPass::Create(sContext, sGeometryPipeline, geometryAttachments, 6);

		const Attachment shadowAttachments[] = {
			{ AttachmentType::Depth, ImageFormat::D32, AR_PBR_SAMPLE_COUNT }
		};

		FramebufferInfo shadowFramebufferInfo{};
		shadowFramebufferInfo.Width = AR_PBR_SHADOW_MAP_WIDTH;
		shadowFramebufferInfo.Height = AR_PBR_SHADOW_MAP_HEIGHT;
		shadowFramebufferInfo.Attachments = shadowAttachments;
		shadowFramebufferInfo.AttachmentCount = 1;

		sShadowMap = Framebuffer::Create(sContext, shadowFramebufferInfo);

		Descriptor shadowDescriptors[] = {
			{ 0, DescriptorType::UniformBuffer },
			{ 1, DescriptorType::UniformBuffer },
		};

		InputLayout shadowInputLayout = {
			{ InputAttribute::Position, 1, InputElementType::Vector3f32 },
			{ InputAttribute::Normal, 1, InputElementType::Vector3f32 },
			{ InputAttribute::UV, 1, InputElementType::Vector2f32 },
			{ InputAttribute::Tangent, 1, InputElementType::Vector3f32 },
			{ InputAttribute::Bitangent, 1, InputElementType::Vector3f32 },
		};

		vertexShaderBinary = ReadFileBinary(AR_SHADOW_VERTEX_SHADER_PATH, &vertexBinarySize);
		fragmentShaderBinary = ReadFileBinary(AR_SHADOW_FRAGMENT_SHADER_PATH, &fragmentBinarySize);

		PipelineInfo shadowPipelineInfo = PipelineInfo::CreateWithDefaultInfo();
		shadowPipelineInfo.Descriptors = shadowDescriptors;
		shadowPipelineInfo.DescriptorCount = 3;
		shadowPipelineInfo.Layout = shadowInputLayout;
		shadowPipelineInfo.VertexShaderBinary = vertexShaderBinary;
		shadowPipelineInfo.VertexShaderSize = vertexBinarySize;
		shadowPipelineInfo.FragmentShaderBinary = fragmentShaderBinary;
		shadowPipelineInfo.FragmentShaderSize = fragmentBinarySize;
		shadowPipelineInfo.Viewport.Size = Vector2(AR_PBR_SHADOW_MAP_WIDTH, AR_PBR_SHADOW_MAP_HEIGHT);
		shadowPipelineInfo.SampleCount = AR_PBR_SAMPLE_COUNT;

		sShadowPipeline = Pipeline::Create(sContext, shadowPipelineInfo);
		sShadowPipeline.SetUniformBuffer(0, sObjectBuffer);
		sShadowPipeline.SetUniformBuffer(1, sShadowPassBuffer);

		free(vertexShaderBinary);
		free(fragmentShaderBinary);

		sShadowPass = RenderPass::Create(sContext, sShadowPipeline, shadowAttachments, 1);

		const Attachment lightAttachments[] = {
			{ AttachmentType::Color, ImageFormat::RGB8, AR_PBR_SAMPLE_COUNT },
			{ AttachmentType::DepthStencil, ImageFormat::D24S8, AR_PBR_SAMPLE_COUNT }
		};

		FramebufferInfo lightFramebufferInfo{};
		lightFramebufferInfo.Width = 1920;
		lightFramebufferInfo.Height = 1080;
		lightFramebufferInfo.Attachments = lightAttachments;
		lightFramebufferInfo.AttachmentCount = 2;

		sLightFramebuffer = Framebuffer::Create(sContext, lightFramebufferInfo);

		Descriptor lightDescriptors[] = {
			{ 0, DescriptorType::UniformBuffer },
			{ 1, DescriptorType::UniformBuffer },

			{ 0, DescriptorType::CombinedImageSampler },
			{ 1, DescriptorType::CombinedImageSampler },
			{ 2, DescriptorType::CombinedImageSampler },
			{ 3, DescriptorType::CombinedImageSampler },
			{ 4, DescriptorType::CombinedImageSampler },
			{ 5, DescriptorType::CombinedImageSampler }
		};

		InputLayout lightInputLayout = {
			{ InputAttribute::Position, 1, InputElementType::Vector2f32 },
			{ InputAttribute::UV, 1, InputElementType::Vector2f32 },	
		};

		vertexShaderBinary = ReadFileBinary(AR_LIGHT_VERTEX_SHADER_PATH, &vertexBinarySize);
		fragmentShaderBinary = ReadFileBinary(AR_LIGHT_FRAGMENT_SHADER_PATH, &fragmentBinarySize);

		PipelineInfo lightPipelineInfo = PipelineInfo::CreateWithDefaultInfo();
		lightPipelineInfo.Descriptors = lightDescriptors;
		lightPipelineInfo.DescriptorCount = 7;
		lightPipelineInfo.Layout = lightInputLayout;
		lightPipelineInfo.VertexShaderBinary = vertexShaderBinary;
		lightPipelineInfo.VertexShaderSize = vertexBinarySize;
		lightPipelineInfo.FragmentShaderBinary = fragmentShaderBinary;
		lightPipelineInfo.FragmentShaderSize = fragmentBinarySize;
		lightPipelineInfo.SampleCount = AR_PBR_SAMPLE_COUNT;

		sLightPipeline = Pipeline::Create(sContext, lightPipelineInfo);
		sLightPipeline.SetUniformBuffer(0, sCameraBuffer);
		sLightPipeline.SetUniformBuffer(1, sLightBuffer);

		free(vertexShaderBinary);
		free(fragmentShaderBinary);

		sLightRenderPass = RenderPass::Create(sContext, sLightPipeline, lightAttachments, 2);

		const Attachment postProcessAttachments[] = {
			{ AttachmentType::Color, ImageFormat::RGB8, AR_PBR_SAMPLE_COUNT },
			{ AttachmentType::DepthStencil, ImageFormat::D24S8, AR_PBR_SAMPLE_COUNT }
		};

		FramebufferInfo postProcessFramebufferInfo{};
		postProcessFramebufferInfo.Width = 1920;
		postProcessFramebufferInfo.Height = 1080;
		postProcessFramebufferInfo.Attachments = postProcessAttachments;
		postProcessFramebufferInfo.AttachmentCount = 2;

		sPostProcessFramebuffer = Framebuffer::Create(sContext, postProcessFramebufferInfo);

		Descriptor postProcessDescriptors[] = {
			{ 0, DescriptorType::UniformBuffer },
			{ 0, DescriptorType::CombinedImageSampler }
		};

		InputLayout postProcessInputLayout = {
			{ InputAttribute::Position, 1, InputElementType::Vector3f32 },
			{ InputAttribute::UV, 1, InputElementType::Vector2f32 },
		};

		vertexShaderBinary = ReadFileBinary(AR_POST_PROCESS_VERTEX_SHADER_PATH, &vertexBinarySize);
		fragmentShaderBinary = ReadFileBinary(AR_POST_PROCESS_FRAGMENT_SHADER_PATH, &fragmentBinarySize);

		PipelineInfo postProcessPipelineInfo = PipelineInfo::CreateWithDefaultInfo();
		postProcessPipelineInfo.Descriptors = postProcessDescriptors;
		postProcessPipelineInfo.DescriptorCount = 2;
		postProcessPipelineInfo.Layout = postProcessInputLayout;
		postProcessPipelineInfo.VertexShaderBinary = vertexShaderBinary;
		postProcessPipelineInfo.VertexShaderSize = vertexBinarySize;
		postProcessPipelineInfo.FragmentShaderBinary = fragmentShaderBinary;
		postProcessPipelineInfo.FragmentShaderSize = fragmentBinarySize;
		postProcessPipelineInfo.SampleCount = AR_PBR_SAMPLE_COUNT;

		sPostProcessPipeline = Pipeline::Create(sContext, postProcessPipelineInfo);
		sPostProcessPipeline.SetUniformBuffer(0, sPostProcessSettingsBuffer);
		free(vertexShaderBinary);
		free(fragmentShaderBinary);

		sPostProcessRenderPass = RenderPass::Create(sContext, sPostProcessPipeline, postProcessAttachments, 2);
	}

	void PBRRenderer::Shutdown() {
		AR_PROFILE_FUNCTION();
	}

	void PBRRenderer::Reload() {
		AR_PROFILE_FUNCTION();
	}

	void PBRRenderer::Begin(const RenderCamera &camera) {
		AR_PROFILE_FUNCTION();

		sCamera = camera.GetCamera();

		sCameraData.Projection = Matrix4::Transpose(camera.GetCamera().GetProjectionMatrix());
		sCameraData.View = Matrix4::Transpose(camera.GetCamera().GetViewMatrix());
		sCameraData.Position = Vector4(camera.GetCamera().Position, 1.0);

		sCameraBuffer.SetData((const void*)&sCameraData);

		sPostProcessSettingsData.Exposure = camera.GetExposure();
		sPostProcessSettingsData.Gamma = camera.GetGamma();

		sClearColor = camera.GetBackgroundColor();
	}

	void PBRRenderer::AddLight(const Vector3 &position, const PointLight &light) {
		AR_PROFILE_FUNCTION();
		uint32_t index = sLightData.PointLightCount++;
		sLightData.PointLights[index].Color = light.Color;
		sLightData.PointLights[index].Intensity = light.Intensity;
		sLightData.PointLights[index].Position = Vector4(position, 1.0);
	}

	void PBRRenderer::AddLight(const Vector3 &direction, const DirectionalLight &light) {
		AR_PROFILE_FUNCTION();
		sLightData.DirectionalLight.Color = light.Color;
		sLightData.DirectionalLight.Direction = Vector4(Vector3::Normalize(direction), 1.0);

		sShadowPassData.LightProjection = Matrix4::Transpose(
			Matrix4::OrthoLH_ZO(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 10.0f)
		);
		sShadowPassData.LightView = Matrix4::Transpose(
			Matrix4::LookAtLH(Vector3::Normalize(-direction) * 5.0f, Vector3::Normalize(direction), Vector3(0.0f, 1.0f, 0.0f))	
		);

		sShadowPassBuffer.SetData(&sShadowPassData);
	}

	void PBRRenderer::Submit(const Transform &transform, const Mesh &mesh, const PBRMaterial &material) {
		AR_PROFILE_FUNCTION();
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
		AR_PROFILE_FUNCTION();
		const Vector2 size = sContext.GetWindow().GetClientSize();

		sRendererAPI.SetViewport(size);
		sRendererAPI.SetScissor(size);
		sGeometryFramebuffer.Resize(size);
		sLightFramebuffer.Resize(size);
		sPostProcessFramebuffer.Resize(size);
		
		sRendererAPI.Begin();

		sRendererAPI.SetClearColor(sClearColor);

		{
			AR_PROFILE_SCOPE("Geometry Pass");
			sRendererAPI.BeginRenderPass(sGeometryRenderPass, sGeometryFramebuffer);
			sRendererAPI.Clear();

			for (const RenderSubmission &submission : sRenderSubmissions) {
				sObjectData.Model = Matrix4::Transpose(submission.Model);
				sObjectData.MVP = Matrix4::Transpose(sCamera.GetProjectionMatrix() * sCamera.GetViewMatrix() * submission.Model);
				sObjectData.Position = Vector4(submission.Position, 1.0);

				sObjectBuffer.SetData((const void*)&sObjectData);
	
				sGeometryPipeline.SetCombinedImageSampler(0, submission.AlbedoMap, sDefaultSampler);
				sGeometryPipeline.SetCombinedImageSampler(1, submission.NormalMap, sDefaultSampler);
				sGeometryPipeline.SetCombinedImageSampler(2, submission.MetallicMap, sDefaultSampler);
				sGeometryPipeline.SetCombinedImageSampler(3, submission.RoughnessMap, sDefaultSampler);
				sGeometryPipeline.SetCombinedImageSampler(4, submission.AmbientOcclusionMap, sDefaultSampler);
	
				sRendererAPI.SetMesh(submission.Mesh);
				sRendererAPI.DrawIndexed(1, submission.Mesh.GetIndexBuffer().GetSize() / sGeometryPipeline.GetElementSize());
			}
			
			sRendererAPI.EndRenderPass();
		}

		{
			AR_PROFILE_SCOPE("Shadow Pass");
			sRendererAPI.BeginRenderPass(sShadowPass, sShadowMap);
			sRendererAPI.Clear();

			for (const RenderSubmission &submission : sRenderSubmissions) {
				sObjectData.Model = Matrix4::Transpose(submission.Model);
				sObjectData.MVP = Matrix4::Transpose(sCamera.GetProjectionMatrix() * sCamera.GetViewMatrix() * submission.Model);
				sObjectData.Position = Vector4(submission.Position, 1.0);

				sObjectBuffer.SetData((const void*)&sObjectData);
				sRendererAPI.SetMesh(submission.Mesh);
				sRendererAPI.DrawIndexed(1, submission.Mesh.GetIndexBuffer().GetSize() / sShadowPipeline.GetElementSize());
			}
			
			sRendererAPI.EndRenderPass();
		}

		{
			AR_PROFILE_SCOPE("Light Pass");
			sRendererAPI.BeginRenderPass(sLightRenderPass, sLightFramebuffer);
			sRendererAPI.Clear();

			sLightBuffer.SetData((const void*)&sLightData);
			sLightPipeline.SetCombinedImageSampler(0, sGeometryFramebuffer.GetColorBuffer(0), sDefaultSampler);
			sLightPipeline.SetCombinedImageSampler(1, sGeometryFramebuffer.GetColorBuffer(1), sDefaultSampler);
			sLightPipeline.SetCombinedImageSampler(2, sGeometryFramebuffer.GetColorBuffer(2), sDefaultSampler);
			sLightPipeline.SetCombinedImageSampler(3, sGeometryFramebuffer.GetColorBuffer(3), sDefaultSampler);
			sLightPipeline.SetCombinedImageSampler(4, sGeometryFramebuffer.GetColorBuffer(4), sDefaultSampler);
			sLightPipeline.SetCombinedImageSampler(5, sShadowMap.GetDepthBuffer(), sDefaultSampler);

			sRendererAPI.SetMesh(sQuadMesh);
			sRendererAPI.DrawIndexed(1, sQuadMesh.GetIndexBuffer().GetSize() / sLightPipeline.GetElementSize());
			
			sRendererAPI.EndRenderPass();
		}

		{
			AR_PROFILE_SCOPE("Post-Process Pass");
			sRendererAPI.BeginRenderPass(sPostProcessRenderPass, sPostProcessFramebuffer);
			sRendererAPI.Clear();

			sPostProcessSettingsBuffer.SetData((const void*)&sPostProcessSettingsData);
			sPostProcessPipeline.SetCombinedImageSampler(0, sLightFramebuffer.GetColorBuffer(0), sDefaultSampler);
			sRendererAPI.SetMesh(sQuadMesh);
			sRendererAPI.DrawIndexed(1, sQuadMesh.GetIndexBuffer().GetSize() / sPostProcessPipeline.GetElementSize());

			sRendererAPI.EndRenderPass();
		}

		sRendererAPI.End();
		
		sRenderSubmissions.clear();

		std::memset(&sLightData, 0, sizeof(sLightData));
		std::memset(&sObjectData, 0, sizeof(sObjectData));
		std::memset(&sCameraData, 0, sizeof(sCameraData));
	}

	RendererAPI PBRRenderer::GetRenderer() {
		return sRendererAPI;
	}


}