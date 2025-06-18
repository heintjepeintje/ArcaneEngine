#include "PostProcessPass.hpp"	

namespace Arcane {

	PostProcessPass::PostProcessPass(const GraphicsContext &context, const RendererAPI &rendererApi, const Buffer &postProcessSettings, const Sampler &sampler) : mContext(context), mRendererAPI(rendererApi), mSampler(sampler) {
		CompileShader(
			mContext,
			"Engine/Shaders/PostProcess/Source/PostProcessShader.vert",
			"Engine/Shaders/PostProcess/Binaries/Output/PostProcessShader.vert.spv"
		);
		CompileShader(
			mContext,
			"Engine/Shaders/PostProcess/Source/PostProcessShader.frag",
			"Engine/Shaders/PostProcess/Binaries/Output/PostProcessShader.frag.spv"
		);

		const ImageFormat postProcessAttachments[] = {
			ImageFormat::RGB8,
			ImageFormat::D24S8
		};

		FramebufferInfo postProcessFramebufferInfo{};
		postProcessFramebufferInfo.Width = 1920;
		postProcessFramebufferInfo.Height = 1080;
		postProcessFramebufferInfo.Attachments = postProcessAttachments;
		postProcessFramebufferInfo.AttachmentCount = 2;
		postProcessFramebufferInfo.Samples = AR_SAMPLE_COUNT;
		postProcessFramebufferInfo.FixedSampleLocations = true;

		mFramebuffer = Framebuffer::Create(mContext, postProcessFramebufferInfo);

		Descriptor postProcessDescriptors[] = {
			{ 0, DescriptorType::UniformBuffer },
			{ 0, DescriptorType::CombinedImageSampler }
		};

		InputLayout postProcessInputLayout = {
			{ InputAttribute::Position, 1, InputElementType::Vector3f32, false },
			{ InputAttribute::UV, 1, InputElementType::Vector2f32, false },
		};

		BufferRef vertexShaderBinary = ReadFileBinary("Engine/Shaders/PostProcess/Binaries/Output/PostProcessShader.vert.spv");
		BufferRef fragmentShaderBinary = ReadFileBinary("Engine/Shaders/PostProcess/Binaries/Output/PostProcessShader.frag.spv");

		PipelineInfo postProcessPipelineInfo = PipelineInfo::CreateWithDefaultInfo();
		postProcessPipelineInfo.Descriptors = postProcessDescriptors;
		postProcessPipelineInfo.DescriptorCount = 2;
		postProcessPipelineInfo.Layout = postProcessInputLayout;
		postProcessPipelineInfo.VertexShaderBinary = vertexShaderBinary;
		postProcessPipelineInfo.FragmentShaderBinary = fragmentShaderBinary;
		postProcessPipelineInfo.SampleCount = AR_SAMPLE_COUNT;

		mPipeline = Pipeline::Create(mContext, postProcessPipelineInfo);
		mPipeline.SetUniformBuffer(0, postProcessSettings);

		mRenderPass = RenderPass::Create(mContext, mPipeline, postProcessAttachments, 2);
	}
		
	void PostProcessPass::Execute(const Camera &camera, const Mesh &quadMesh, const Framebuffer &lightFramebuffer) {
		mFramebuffer.Resize(mContext.GetWindow().GetClientSize());
		
		mRendererAPI.BeginRenderPass(mRenderPass, mFramebuffer);
		mRendererAPI.Clear();

		mPipeline.SetCombinedImageSampler(0, lightFramebuffer.GetColorTexture(0), mSampler);
		
		mRendererAPI.SetMesh(quadMesh);
		mRendererAPI.DrawIndexed(1, quadMesh.GetIndexBuffer().GetSize() / mPipeline.GetElementSize());

		mRendererAPI.EndRenderPass();
	}

}