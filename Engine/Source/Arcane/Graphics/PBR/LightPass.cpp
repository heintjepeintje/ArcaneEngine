#include "LightPass.hpp"

#include <Arcane/Graphics/Loader.hpp>

namespace Arcane {

	LightPass::LightPass(const GraphicsContext &context, const RendererAPI &rendererApi, const Buffer &cameraData, const Buffer &lightData, const Sampler &sampler, const Sampler &shadowSampler) : mContext(context), mRendererAPI(rendererApi), mSampler(sampler), mShadowSampler(shadowSampler) {		
		CompileShader(mContext,
			"Engine/Shaders/Light/Source/LightShader.vert",
			"Engine/Shaders/Light/Binaries/Output/LightShader.vert.spv"
		);

		CompileShader(mContext,
			"Engine/Shaders/Light/Source/LightShader.frag",
			"Engine/Shaders/Light/Binaries/Output/LightShader.frag.spv"
		);

		const ImageFormat lightAttachments[] = {
			ImageFormat::RGB8,
			ImageFormat::D24S8
		};

		FramebufferInfo lightFramebufferInfo{};
		lightFramebufferInfo.Width = 1920;
		lightFramebufferInfo.Height = 1080;
		lightFramebufferInfo.Attachments = lightAttachments;
		lightFramebufferInfo.AttachmentCount = 2;
		lightFramebufferInfo.Samples = AR_SAMPLE_COUNT;
		lightFramebufferInfo.FixedSampleLocations = true;

		mFramebuffer = Framebuffer::Create(mContext, lightFramebufferInfo);

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
			{ InputAttribute::Position, 1, InputElementType::Vector2f32, false },
			{ InputAttribute::UV, 1, InputElementType::Vector2f32, false },	
		};

		BufferRef vertexShaderBinary = ReadFileBinary("Engine/Shaders/Light/Binaries/Output/LightShader.vert.spv");
		BufferRef fragmentShaderBinary = ReadFileBinary("Engine/Shaders/Light/Binaries/Output/LightShader.frag.spv");

		PipelineInfo lightPipelineInfo = PipelineInfo::CreateWithDefaultInfo();
		lightPipelineInfo.Descriptors = lightDescriptors;
		lightPipelineInfo.DescriptorCount = 8;
		lightPipelineInfo.Layout = lightInputLayout;
		lightPipelineInfo.VertexShaderBinary = vertexShaderBinary;
		lightPipelineInfo.FragmentShaderBinary = fragmentShaderBinary;
		lightPipelineInfo.SampleCount = AR_SAMPLE_COUNT;

		mPipeline = Pipeline::Create(mContext, lightPipelineInfo);
		mPipeline.SetUniformBuffer(0, cameraData);
		mPipeline.SetUniformBuffer(1, lightData);

		mRenderPass = RenderPass::Create(mContext, mPipeline, lightAttachments, 2);
	}

	void LightPass::Execute(const Camera &camera, const Mesh &quadMesh, const Framebuffer &geometryFramebuffer, const Framebuffer &shadowFramebuffer) {
		mFramebuffer.Resize(mContext.GetWindow().GetClientSize());

		mRendererAPI.BeginRenderPass(mRenderPass, mFramebuffer);
		mRendererAPI.Clear();

		mPipeline.SetCombinedImageSampler(0, geometryFramebuffer.GetColorTexture(0), mSampler);
		mPipeline.SetCombinedImageSampler(1, geometryFramebuffer.GetColorTexture(1), mSampler);
		mPipeline.SetCombinedImageSampler(2, geometryFramebuffer.GetColorTexture(2), mSampler);
		mPipeline.SetCombinedImageSampler(3, geometryFramebuffer.GetColorTexture(3), mSampler);
		mPipeline.SetCombinedImageSampler(4, geometryFramebuffer.GetColorTexture(4), mSampler);
		mPipeline.SetCombinedImageSampler(5, shadowFramebuffer.GetDepthTexture(), mShadowSampler);

		mRendererAPI.SetMesh(quadMesh);
		mRendererAPI.DrawIndexed(1, quadMesh.GetIndexBuffer().GetSize() / mPipeline.GetElementSize());
		
		mRendererAPI.EndRenderPass();
	}

}