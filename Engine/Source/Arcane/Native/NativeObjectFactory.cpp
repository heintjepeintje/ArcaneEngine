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
#include <Platform/Windows/WindowsSocket.hpp>

#include <Platform/OpenGL/OpenGLBuffer.hpp>
#include <Platform/OpenGL/OpenGLFramebuffer.hpp>
#include <Platform/OpenGL/OpenGLGraphicsContext.hpp>
#include <Platform/OpenGL/OpenGLMesh.hpp>
#include <Platform/OpenGL/OpenGLPipeline.hpp>
#include <Platform/OpenGL/OpenGLRenderer.hpp>
#include <Platform/OpenGL/OpenGLRenderPass.hpp>
#include <Platform/OpenGL/OpenGLTexture.hpp>

#include <Platform/Vulkan/VulkanGraphicsContext.hpp>

namespace Arcane {

	Ref<NativeWindow> NativeWindow::Create(uint32_t width, uint32_t height, const std::string &title) {
#ifdef _WIN32
		return CastRef<NativeWindow>(CreateRef<Win32Window>(width, height, title));
#else
		return Ref<NativeWindow>::Invalid();
#endif
	}

	Ref<NativeSocket> NativeSocket::Create() {
#ifdef _WIN32
		return CastRef<NativeSocket>(CreateRef<WindowsSocket>());
#else
		return Ref<NativeSocket>::Invalid();
#endif
	}

	Ref<NativeBuffer> NativeBuffer::Create(const Ref<NativeGraphicsContext> &context, size_t size, uint32_t flags) {
		switch (context->GetGraphicsAPI()) {
			case GraphicsAPI::OpenGL: return CastRef<NativeBuffer>(CreateRef<OpenGLBuffer>(CastRef<OpenGLGraphicsContext>(context), size, flags));
			default: return Ref<NativeBuffer>::Invalid();
		}
	}

	Ref<NativeFramebuffer> NativeFramebuffer::Create(const Ref<NativeGraphicsContext> &context, const FramebufferInfo &info) {
		switch (context->GetGraphicsAPI()) {
			case GraphicsAPI::OpenGL: return CastRef<NativeFramebuffer>(CreateRef<OpenGLFramebuffer>(CastRef<OpenGLGraphicsContext>(context), info));
			default: return Ref<NativeFramebuffer>::Invalid();
		}
	}

	Ref<NativeGraphicsContext> NativeGraphicsContext::Create(const Ref<NativeWindow> &window, GraphicsAPI api) {
		switch (api) {
			case GraphicsAPI::OpenGL: return CastRef<NativeGraphicsContext>(CreateRef<OpenGLGraphicsContext>(window));
			case GraphicsAPI::Vulkan: return CastRef<NativeGraphicsContext>(CreateRef<VulkanGraphicsContext>(window));
			default: return Ref<NativeGraphicsContext>::Invalid();
		}
	}

	Ref<NativeMesh> NativeMesh::Create(const Ref<NativeGraphicsContext> &context) {
		switch (context->GetGraphicsAPI()) {
			case GraphicsAPI::OpenGL: return CastRef<NativeMesh>(CreateRef<OpenGLMesh>(CastRef<OpenGLGraphicsContext>(context)));
			default: return Ref<NativeMesh>::Invalid();
		}
	}

	Ref<NativePipeline> NativePipeline::Create(const Ref<NativeGraphicsContext> &context, const PipelineInfo &info) {
		switch (context->GetGraphicsAPI()) {
			case GraphicsAPI::OpenGL: return CastRef<NativePipeline>(CreateRef<OpenGLPipeline>(CastRef<OpenGLGraphicsContext>(context), info));
			default: return Ref<NativePipeline>::Invalid();
		}
	}

	Ref<NativeRendererAPI> NativeRendererAPI::Create(const Ref<NativeGraphicsContext> &context) {
		switch (context->GetGraphicsAPI()) {
			case GraphicsAPI::OpenGL: return CastRef<NativeRendererAPI>(CreateRef<OpenGLRendererAPI>(CastRef<OpenGLGraphicsContext>(context)));
			default: return Ref<NativeRendererAPI>::Invalid();
		}
	}

	Ref<NativeRenderPass> NativeRenderPass::Create(const Ref<NativeGraphicsContext> &context, const Ref<NativePipeline> &pipeline, const ImageFormat *attachments, size_t attachmentCount) {
		switch (context->GetGraphicsAPI()) {
			case GraphicsAPI::OpenGL: return CastRef<NativeRenderPass>(CreateRef<OpenGLRenderPass>(CastRef<OpenGLGraphicsContext>(context),  CastRef<OpenGLPipeline>(pipeline), attachments, attachmentCount));
			default: return Ref<NativeRenderPass>::Invalid();
		}
	}

	Ref<NativeTexture> NativeTexture::Create(const Ref<NativeGraphicsContext> &context, const TextureInfo &info) {
		switch (context->GetGraphicsAPI()) {
			case GraphicsAPI::OpenGL: return CastRef<NativeTexture>(CreateRef<OpenGLTexture>(CastRef<OpenGLGraphicsContext>(context), info));
			default: return Ref<NativeTexture>::Invalid();
		}
	}

	Ref<NativeSampler> NativeSampler::Create(const Ref<NativeGraphicsContext> &context, const SamplerInfo &info) {
		switch (context->GetGraphicsAPI()) {
			case GraphicsAPI::OpenGL: return CastRef<NativeSampler>(CreateRef<OpenGLSampler>(CastRef<OpenGLGraphicsContext>(context), info));
			default: return Ref<NativeSampler>::Invalid();
		}
	}

}