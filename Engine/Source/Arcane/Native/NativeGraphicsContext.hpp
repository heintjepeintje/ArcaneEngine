#pragma once

#include <Arcane/Core.hpp>
#include "NativeWindow.hpp"
#include <Arcane/Math/Rect2D.hpp>

namespace Arcane {

	enum class GraphicsAPI {
		None = 0,
		OpenGL, Vulkan
	};

	struct GraphicsLimits {
		uint32_t MaxShaderStorageBlocks;
		uint32_t MaxUniformBlocks;
		uint32_t MaxTextureImageUnits;
		
		uint32_t MaxWorkGroupInvocations;
		uint32_t MaxWorkGroupCountX, MaxWorkGroupCountY, MaxWorkGroupCountZ;
		uint32_t MaxWorkGroupSizeX, MaxWorkGroupSizeY, MaxWorkGroupSizeZ;

		uint32_t MaxVertexAttributes;

		uint32_t MaxCubeMapTextureSize;
		uint32_t Max3DTextureSize;
		uint32_t MaxTextureSize;
		uint32_t MaxTextureLODBias;
		uint32_t MaxArrayTextureLayers;
		uint32_t MaxColorSamples;
		uint32_t MaxDepthSamples;

		uint32_t MaxUniformBlockSize;
		uint32_t MaxStorageBlockSize;

		uint32_t MaxClipDistances;
		uint32_t MaxViewportWidth, MaxViewportHeight;
		uint32_t MaxViewports;
		uint32_t MaxDrawBuffers;

		uint32_t MaxFramebufferWidth;
		uint32_t MaxFramebufferHeight;
		uint32_t MaxFramebufferLayers;
		uint32_t MaxFramebufferSamples;
	};

	class NativeGraphicsContext {
	public:
		static Ref<NativeGraphicsContext> Create(const Ref<NativeWindow> &window, GraphicsAPI api);

	public:
		NativeGraphicsContext() { }
		virtual ~NativeGraphicsContext() { }

		virtual void Present() = 0;
		
		virtual uint32_t GetVersionMajor() const = 0;
		virtual uint32_t GetVersionMinor() const = 0;
		virtual uint32_t GetPatchLevel() const = 0;
		virtual const char *GetDeviceName() const = 0;
		virtual Ref<NativeWindow> GetWindow() const = 0;
		virtual GraphicsAPI GetGraphicsAPI() const = 0;
		virtual const GraphicsLimits &GetGraphicsLimits() const = 0;
	};

}