#include <Arcane/Core.hpp>

#include <Arcane/Native/NativeBuffer.hpp>
#include <Arcane/Native/NativeFramebuffer.hpp>
#include <Arcane/Native/NativeGraphicsContext.hpp>
#include <Arcane/Native/NativeMesh.hpp>
#include <Arcane/Native/NativePipeline.hpp>
#include <Arcane/Native/NativeRenderer.hpp>
#include <Arcane/Native/NativeRenderPass.hpp>
#include <Arcane/Native/NativeTexture.hpp>
#include <Arcane/Native/NativeWindow.hpp>

#include <Platform/Windows/Win32Window.hpp>

#include <Platform/OpenGL/OpenGLBuffer.hpp>
#include <Platform/OpenGL/OpenGLFramebuffer.hpp>
#include <Platform/OpenGL/OpenGLGraphicsContext.hpp>
#include <Platform/OpenGL/OpenGLMesh.hpp>
#include <Platform/OpenGL/OpenGLPipeline.hpp>
#include <Platform/OpenGL/OpenGLRenderer.hpp>
#include <Platform/OpenGL/OpenGLRenderPass.hpp>
#include <Platform/OpenGL/OpenGLTexture.hpp>

namespace Arcane {

	Ref<NativeWindow> NativeWindow::Create(uint32_t width, uint32_t height, const std::string &title) {
		return CastRef<NativeWindow>(CreateRef<Win32Window>(
			width, height, 
			title
		));
	}

	Ref<NativeBuffer> NativeBuffer::Create(const Ref<NativeGraphicsContext> &context, size_t size, uint32_t flags) {
		return CastRef<NativeBuffer>(CreateRef<OpenGLBuffer>(
			size, flags
		));
	}

	Ref<NativeFramebuffer> NativeFramebuffer::Create(const Ref<NativeGraphicsContext> &context, const FramebufferInfo &info) {
		return CastRef<NativeFramebuffer>(CreateRef<OpenGLFramebuffer>(
			CastRef<OpenGLGraphicsContext>(context),
			info
		));
	}

	Ref<NativeGraphicsContext> NativeGraphicsContext::Create(const Ref<NativeWindow> &window) {
		return CastRef<NativeGraphicsContext>(CreateRef<OpenGLGraphicsContext>(
			window
		));
	}

	Ref<NativeMesh> NativeMesh::Create(const Ref<NativeGraphicsContext> &context) {
		return CastRef<NativeMesh>(CreateRef<OpenGLMesh>(
			CastRef<OpenGLGraphicsContext>(context)
		));
	}

	Ref<NativePipeline> NativePipeline::Create(const Ref<NativeGraphicsContext> &context, const PipelineInfo &info) {
		return CastRef<NativePipeline>(CreateRef<OpenGLPipeline>(
			CastRef<OpenGLGraphicsContext>(context),
			info
		));
	}

	Ref<NativeRendererAPI> NativeRendererAPI::Create(const Ref<NativeGraphicsContext> &context) {
		return CastRef<NativeRendererAPI>(CreateRef<OpenGLRendererAPI>(
			CastRef<OpenGLGraphicsContext>(context)
		));
	}

	Ref<NativeRenderPass> NativeRenderPass::Create(const Ref<NativeGraphicsContext> &context, const Ref<NativePipeline> &pipeline, const Attachment *attachments, size_t attachmentCount) {
		return CastRef<NativeRenderPass>(CreateRef<OpenGLRenderPass>(
			CastRef<OpenGLGraphicsContext>(context), 
			CastRef<OpenGLPipeline>(pipeline),
			attachments,
			attachmentCount
		));
	}

	Ref<NativeTexture> NativeTexture::Create(const Ref<NativeGraphicsContext> &context, const TextureInfo &info) {
		return CastRef<NativeTexture>(CreateRef<OpenGLTexture>(
			CastRef<OpenGLGraphicsContext>(context), 
			info
		));
	}

	Ref<NativeSampler> NativeSampler::Create(const Ref<NativeGraphicsContext> &context, const SamplerInfo &info) {
		return CastRef<NativeSampler>(CreateRef<OpenGLSampler>(
			CastRef<OpenGLGraphicsContext>(context), 
			info
		));
	}

}