#include "GeometryPass.hpp"

namespace Arcane {

	GeometryPass::GeometryPass(const GraphicsContext &context, const RendererAPI &rendererApi, const Buffer &cameraData, const Buffer &objectData, const Buffer &shadowData, const Sampler &sampler) : mContext(context), mRendererAPI(rendererApi), mObjectBuffer(objectData), mSampler(sampler) {
		CompileShader(
			mContext, 
			"Engine/Shaders/Geometry/Source/GeometryShader.vert", 
			"Engine/Shaders/Geometry/Binaries/Output/GeometryShader.vert.spv"
		);
		CompileShader(
			mContext,
			"Engine/Shaders/Geometry/Source/GeometryShader.frag",
			"Engine/Shaders/Geometry/Binaries/Output/GeometryShader.frag.spv"
		);
		
		const ImageFormat geometryAttachments[] = {
			ImageFormat::RGB32F,
			ImageFormat::RGB32F,
			ImageFormat::RGB32F,
			ImageFormat::RGB32F,
			ImageFormat::RGB32F,
			ImageFormat::D24S8,
		};

		FramebufferInfo geometryFramebufferInfo{};
		geometryFramebufferInfo.Width = 1920;
		geometryFramebufferInfo.Height = 1080;
		geometryFramebufferInfo.Attachments = geometryAttachments;
		geometryFramebufferInfo.AttachmentCount = 6;
		geometryFramebufferInfo.Samples = 1;
		geometryFramebufferInfo.FixedSampleLocations = true;

		mFramebuffer = Framebuffer::Create(mContext, geometryFramebufferInfo);

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
			{ InputAttribute::Position, 1, InputElementType::Vector3f32, false },
			{ InputAttribute::Normal, 1, InputElementType::Vector3f32, false },
			{ InputAttribute::UV, 1, InputElementType::Vector2f32, false },
			{ InputAttribute::Tangent, 1, InputElementType::Vector3f32, false },
			{ InputAttribute::Bitangent, 1, InputElementType::Vector3f32, false },
		};
		
		ShaderBinary vertexShaderBinary = ReadShaderBinary("Engine/Shaders/Geometry/Binaries/Output/GeometryShader.vert.spv");
		ShaderBinary fragmentShaderBinary = ReadShaderBinary("Engine/Shaders/Geometry/Binaries/Output/GeometryShader.frag.spv");
		
		PipelineInfo geometryPipelineInfo = PipelineInfo::CreateWithDefaultInfo();
		geometryPipelineInfo.CullMode = CullMode::None;
		geometryPipelineInfo.Descriptors = geometryDescriptors;
		geometryPipelineInfo.DescriptorCount = 8;
		geometryPipelineInfo.Layout = geometryInputLayout;
		geometryPipelineInfo.VertexShader = vertexShaderBinary;
		geometryPipelineInfo.FragmentShader = fragmentShaderBinary;
		geometryPipelineInfo.SampleCount = 1;

		mPipeline = Pipeline::Create(mContext, geometryPipelineInfo);
		mPipeline.SetUniformBuffer(0, cameraData);
		mPipeline.SetUniformBuffer(1, objectData);
		mPipeline.SetUniformBuffer(2, shadowData);

		free(vertexShaderBinary.Data);
		free(fragmentShaderBinary.Data);

		mRenderPass = RenderPass::Create(mContext, mPipeline, geometryAttachments, 6);
	}

	GeometryPass::~GeometryPass() { }

	void GeometryPass::Execute(const Camera &camera, const std::vector<RenderSubmission> &submissions) {
		mFramebuffer.Resize(mContext.GetWindow().GetClientSize());

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

			mPipeline.SetCombinedImageSampler(0, submission.AlbedoMap, mSampler);
			mPipeline.SetCombinedImageSampler(1, submission.NormalMap, mSampler);
			mPipeline.SetCombinedImageSampler(2, submission.MetallicMap, mSampler);
			mPipeline.SetCombinedImageSampler(3, submission.RoughnessMap, mSampler);
			mPipeline.SetCombinedImageSampler(4, submission.AmbientOcclusionMap, mSampler);

			mRendererAPI.SetMesh(submission.Mesh);
			mRendererAPI.DrawIndexed(1, submission.Mesh.GetIndexBuffer().GetSize() / mPipeline.GetElementSize());
		}
		
		mRendererAPI.EndRenderPass();
	}

}