#include "Renderer.hpp"

#include <Arcane/Graphics/Base/Buffer.hpp>
#include <Arcane/Graphics/Base/RendererAPI.hpp>
#include <Arcane/Graphics/Base/RenderPass.hpp>
#include <Arcane/Graphics/Base/Pipeline.hpp>
#include <Arcane/Util/FileUtil.hpp>
#include <Arcane/System/Time.hpp>
#include "PBRCommon.hpp"

#include "GeometryPass.hpp"
#include "ShadowPass.hpp"
#include "LightPass.hpp"
#include "PostProcessPass.hpp"

namespace Arcane {

	static CameraData sCameraData;
	static PostProcessSettings sPostProcessSettings;
	static LightData sLightData;
	static ShadowPassData sShadowPassData;

	static Camera3D sCamera;

	static GraphicsContext sContext;

	static Buffer sCameraBuffer;
	static Buffer sObjectBuffer;
	static Buffer sLightBuffer;
	static Buffer sShadowBuffer;
	static Buffer sPostProcessSettingsBuffer;
	static RendererAPI sRendererAPI;

	static GeometryPass sGeometryPass;
	static ShadowPass sShadowPass;
	static LightPass sLightPass;
	static PostProcessPass sPostProcessPass;

	static Color sClearColor;
	
	static Mesh sQuadMesh;
	static Sampler sDefaultSampler, sShadowSampler;

	static std::vector<RenderSubmission> sRenderSubmissions;

	static void InitBuffers() {
		sCameraBuffer = Buffer::Create(sContext, sizeof(CameraData), BufferFlag::Static);
		sObjectBuffer = Buffer::Create(sContext, sizeof(ObjectData));
		sShadowBuffer = Buffer::Create(sContext, sizeof(ShadowPassData), BufferFlag::Static);
		sLightBuffer = Buffer::Create(sContext, sizeof(LightData), BufferFlag::Static);
		sPostProcessSettingsBuffer = Buffer::Create(sContext, sizeof(PostProcessSettings), BufferFlag::Static);
	}

	static void InitSamplers() {
		SamplerInfo samplerInfo{};
		samplerInfo.MinFilter = SamplerFilter::Linear; 
		samplerInfo.MagFilter = SamplerFilter::Linear;
		samplerInfo.MipmapFilter = SamplerFilter::Linear;
		samplerInfo.MinLOD = 0.0f;
		samplerInfo.MaxLOD = 0.0f;
		samplerInfo.WrapS = SamplerWrap::Repeat;
		samplerInfo.WrapR = SamplerWrap::Repeat;
		samplerInfo.WrapT = SamplerWrap::Repeat;
		samplerInfo.BorderColor = Color::Black();

		sDefaultSampler = Sampler::Create(sContext, samplerInfo);

		SamplerInfo shadowSamplerInfo{};
		shadowSamplerInfo.MinFilter = SamplerFilter::Nearest;
		shadowSamplerInfo.MagFilter = SamplerFilter::Nearest;
		shadowSamplerInfo.MipmapFilter = SamplerFilter::None;
		shadowSamplerInfo.MinLOD = 0.0f;
		shadowSamplerInfo.MaxLOD = 0.0f;
		shadowSamplerInfo.WrapS = SamplerWrap::ClampToBorder;
		shadowSamplerInfo.WrapR = SamplerWrap::ClampToBorder;
		shadowSamplerInfo.WrapT = SamplerWrap::ClampToBorder;
		shadowSamplerInfo.BorderColor = Color::White();

		sShadowSampler = Sampler::Create(sContext, samplerInfo);
	}

	static void InitFullscreenQuad() {
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
	}

	static void InitPostProcessPass() {
	}

	void Renderer::Init(const GraphicsContext &context) {
		AR_PROFILE_FUNCTION();
		sContext = context;

		sRendererAPI = RendererAPI::Create(sContext);

		InitBuffers();
		InitSamplers();
		InitFullscreenQuad();

		Reload();
	}

	void Renderer::Shutdown() {
		AR_PROFILE_FUNCTION();
	}

	void Renderer::Reload() {
		AR_PROFILE_FUNCTION();

		sGeometryPass = GeometryPass(sContext, sRendererAPI, sCameraBuffer, sObjectBuffer, sShadowBuffer, sDefaultSampler);
		sShadowPass = ShadowPass(sContext, sRendererAPI, sObjectBuffer, sShadowBuffer);
		sLightPass = LightPass(sContext, sRendererAPI, sCameraBuffer, sLightBuffer, sDefaultSampler, sShadowSampler);
		sPostProcessPass = PostProcessPass(sContext, sRendererAPI, sPostProcessSettingsBuffer, sDefaultSampler);
	}

	void Renderer::Begin(const RenderCamera &camera) {
		AR_PROFILE_FUNCTION();

		sCamera = camera.GetCamera();

		sCameraData.Projection = Matrix4::Transpose(camera.GetCamera().GetProjectionMatrix());
		sCameraData.View = Matrix4::Transpose(camera.GetCamera().GetViewMatrix());
		sCameraData.Position = Vector4(camera.GetCamera().Position, 1.0);
		sCameraData.NearPlane = camera.GetCamera().Near;
		sCameraData.FarPlane = camera.GetCamera().Far;

		sPostProcessSettings.Exposure = camera.GetExposure();
		sPostProcessSettings.Gamma = camera.GetGamma();

		sClearColor = camera.GetBackgroundColor();
	}

	void Renderer::AddLight(const Vector3 &position, const PointLight &light) {
		AR_PROFILE_FUNCTION();
		uint32_t index = sLightData.PointLightCount++;
		sLightData.PointLights[index].Color = light.Color;
		sLightData.PointLights[index].Intensity = light.Intensity;
		sLightData.PointLights[index].Position = Vector4(position, 1.0);
	}

	void Renderer::AddLight(const Vector3 &direction, const DirectionalLight &light) {
		AR_PROFILE_FUNCTION();
		sLightData.DirectionalLight.Color = light.Color;
		sLightData.DirectionalLight.Direction = Vector4(Vector3::Normalize(direction), 1.0);

		sShadowPassData.LightProjection = Matrix4::Transpose(
			Matrix4::OrthoLH_ZO(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 30.0f)
		);
		sShadowPassData.LightView = Matrix4::Transpose(
			Matrix4::LookAtLH(Vector3::Normalize(-direction) * 5.0f, Vector3::Normalize(direction), Vector3(0.0f, 1.0f, 0.0f))	
		);

		sShadowBuffer.SetData(sShadowPassData);
	}

	void Renderer::Submit(const Transform &transform, const Mesh &mesh, const Material &material) {
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

	void Renderer::End() {
		AR_PROFILE_FUNCTION();

		sRendererAPI.SetViewport(sContext.GetWindow().GetClientSize());
		sRendererAPI.SetScissor(sContext.GetWindow().GetClientSize());
		
		sCameraBuffer.SetData(sCameraData);
		sLightBuffer.SetData(sLightData);
		sPostProcessSettingsBuffer.SetData(sPostProcessSettings);
		
		sRendererAPI.Begin();

		sRendererAPI.SetClearColor(sClearColor);

		sGeometryPass.Execute(sCamera, sRenderSubmissions);
		sShadowPass.Execute(sCamera, sRenderSubmissions);
		sLightPass.Execute(sCamera, sQuadMesh, sGeometryPass.GetFramebuffer(), sShadowPass.GetFramebuffer());
		sPostProcessPass.Execute(sCamera, sQuadMesh, sLightPass.GetFramebuffer());

		sRendererAPI.End();
		
		sRenderSubmissions.clear();

		std::memset(&sLightData, 0, sizeof(LightData));
		std::memset(&sCameraData, 0, sizeof(CameraData));
		std::memset(&sShadowPassData, 0, sizeof(ShadowPassData));
		std::memset(&sPostProcessSettings, 0, sizeof(PostProcessSettings));
	}

	RendererAPI Renderer::GetRenderer() {
		return sRendererAPI;
	}


}