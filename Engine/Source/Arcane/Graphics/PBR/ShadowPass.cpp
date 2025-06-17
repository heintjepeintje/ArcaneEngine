#include "shadowPass.hpp"

namespace Arcane {

	ShadowPass::ShadowPass(const GraphicsContext &context, const RendererAPI &rendererApi, const Buffer &objectData, const Buffer &shadowData) : mContext(context), mRendererAPI(rendererApi), mObjectBuffer(objectData) {
		CompileShader(
			mContext, 
			"Engine/Shaders/Shadow/Source/ShadowShader.vert", 
			"Engine/Shaders/Shadow/Binaries/Output/ShadowShader.vert.spv"
		);
		CompileShader(
			mContext,
			"Engine/Shaders/Shadow/Source/ShadowShader.frag",
			"Engine/Shaders/Shadow/Binaries/Output/ShadowShader.frag.spv"
		);
		
		const ImageFormat shadowAttachments[] = { ImageFormat::D32 };

		FramebufferInfo shadowFramebufferInfo{};
		shadowFramebufferInfo.Width = AR_SHADOW_MAP_WIDTH;
		shadowFramebufferInfo.Height = AR_SHADOW_MAP_HEIGHT;
		shadowFramebufferInfo.Attachments = shadowAttachments;
		shadowFramebufferInfo.AttachmentCount = 1;
		shadowFramebufferInfo.Samples = AR_SAMPLE_COUNT;
		shadowFramebufferInfo.FixedSampleLocations = true;

		mFramebuffer = Framebuffer::Create(mContext, shadowFramebufferInfo);

		Descriptor shadowDescriptors[] = {
			{ 0, DescriptorType::UniformBuffer },
			{ 1, DescriptorType::UniformBuffer }
		};

		InputLayout shadowInputLayout = {
			{ InputAttribute::Position, 1, InputElementType::Vector3f32, false },
			{ InputAttribute::Normal, 1, InputElementType::Vector3f32, false },
			{ InputAttribute::UV, 1, InputElementType::Vector2f32, false },
			{ InputAttribute::Tangent, 1, InputElementType::Vector3f32, false },
			{ InputAttribute::Bitangent, 1, InputElementType::Vector3f32, false },
		};

		ShaderBinary vertexShaderBinary = ReadShaderBinary("Engine/Shaders/Shadow/Binaries/Output/ShadowShader.vert.spv");
		ShaderBinary fragmentShaderBinary = ReadShaderBinary("Engine/Shaders/Shadow/Binaries/Output/ShadowShader.frag.spv");

		PipelineInfo shadowPipelineInfo = PipelineInfo::CreateWithDefaultInfo();
		shadowPipelineInfo.CullMode = CullMode::None;
		shadowPipelineInfo.Descriptors = shadowDescriptors;
		shadowPipelineInfo.DescriptorCount = 2;
		shadowPipelineInfo.Layout = shadowInputLayout;
		shadowPipelineInfo.VertexShader = vertexShaderBinary;
		shadowPipelineInfo.FragmentShader = fragmentShaderBinary;
		shadowPipelineInfo.Viewport.Size = Vector2(AR_SHADOW_MAP_WIDTH, AR_SHADOW_MAP_HEIGHT);
		shadowPipelineInfo.Scissor.Size = Vector2(AR_SHADOW_MAP_WIDTH, AR_SHADOW_MAP_HEIGHT);
		shadowPipelineInfo.SampleCount = AR_SAMPLE_COUNT;

		mPipeline = Pipeline::Create(mContext, shadowPipelineInfo);
		mPipeline.SetUniformBuffer(0, mObjectBuffer);
		mPipeline.SetUniformBuffer(1, shadowData);

		free(vertexShaderBinary.Data);
		free(fragmentShaderBinary.Data);

		mRenderPass = RenderPass::Create(mContext, mPipeline, shadowAttachments, 1);
	}

	ShadowPass::~ShadowPass() { }

	void ShadowPass::Execute(const Camera &camera, const std::vector<RenderSubmission> &submissions) {
		mRendererAPI.BeginRenderPass(mRenderPass, mFramebuffer);
		mRendererAPI.Clear();

		ObjectData objectData;

		const Matrix4 projection = camera.GetProjectionMatrix();
		const Matrix4 view = camera.GetViewMatrix();

		for (const RenderSubmission &submission : submissions) {
			objectData.Model = Matrix4::Transpose(submission.Model);
			objectData.MVP = Matrix4::Transpose(projection * view * objectData.Model);
			objectData.Position = Vector4(submission.Position, 1.0);

			mObjectBuffer.SetData(objectData);

			mRendererAPI.SetMesh(submission.Mesh);
			mRendererAPI.DrawIndexed(1, submission.Mesh.GetIndexBuffer().GetSize() / mPipeline.GetElementSize());
		}
		
		mRendererAPI.EndRenderPass();
	}

}