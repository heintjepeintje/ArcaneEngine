#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/Base/GraphicsContext.hpp>
#include <Arcane/Graphics/Base/RendererAPI.hpp>
#include <Arcane/Graphics/Base/RenderPass.hpp>
#include <Arcane/Graphics/Base/Framebuffer.hpp>
#include <Arcane/Graphics/Base/Pipeline.hpp>
#include <Arcane/Graphics/Base/Buffer.hpp>
#include <Arcane/Graphics/Camera.hpp>

#include "PBRCommon.hpp"

namespace Arcane {

	class LightPass {
	public:
		LightPass() = default;
		LightPass(const GraphicsContext &context, const RendererAPI &rendererApi, const Buffer &cameraData, const Buffer &lightData, const Sampler &sampler, const Sampler &shadowSampler);
		
		void Execute(const Camera &camera, const Mesh &quadMesh, const Framebuffer &geometryFramebuffer, const Framebuffer &shadowFramebuffer);

		inline Framebuffer &GetFramebuffer() { return mFramebuffer; }
		inline RenderPass &GetRenderPass() { return mRenderPass; }
		inline Pipeline &GetPipeline() { return mPipeline; }
		inline GraphicsContext &GetContext() { return mContext; }

	private:
		GraphicsContext mContext;
		RendererAPI mRendererAPI;

		RenderPass mRenderPass;
		Framebuffer mFramebuffer;
		Pipeline mPipeline;

		Sampler mSampler;
		Sampler mShadowSampler;
	};

}