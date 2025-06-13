#pragma once

#include <Arcane/Core.hpp>
#include <Arcane/Graphics/InputLayout.hpp>
#include <Arcane/Graphics/Shader.hpp>
#include <Arcane/Math/Rect2D.hpp>

#include "NativeGraphicsContext.hpp"
#include "NativeBuffer.hpp"
#include "NativeTexture.hpp"

namespace Arcane {

	enum class CullMode {
		None = 0,
		Front,
		Back,
		FrontAndBack
	};

	enum class WindingOrder {
		None = 0,
		Clockwise,
		CounterClockwise
	};

	enum class FillMode {
		None = 0,
		Solid,
		Wireframe,
		Points
	};

	enum class PrimitiveTopology {
		None = 0,
		TriangleList,
		TriangleStrip,
		LineList,
		LineStrip,
		PointList
	};

	enum class DescriptorType {
		None = 0,
		UniformBuffer, StorageBuffer,
		CombinedImageSampler
	};

	namespace ShaderStage {
		enum _ShaderStage {
			Vertex = AR_BIT(0), 
			Fragment = AR_BIT(1),
			Compute = AR_BIT(2),
			Geometry = AR_BIT(3),
			TessellationControl = AR_BIT(4),
			TessellationEvaluation = AR_BIT(5),
		};
	}

	struct Descriptor {
		uint32_t Binding;
		DescriptorType Type;
	};

	namespace OutputComponent {
		enum _OutputComponent {
			Red = AR_BIT(0),
			Green = AR_BIT(1),
			Blue = AR_BIT(2),
			Alpha = AR_BIT(3),
			Color = 0x0f,
			Depth = AR_BIT(4),
			Stencil = AR_BIT(5),
			All = 0x3f
		};
	}

	struct PipelineInfo {
		static PipelineInfo CreateWithDefaultInfo();

		PipelineInfo() { }
		~PipelineInfo() { }

		uint8_t OutputMask;
		uint32_t StageFlags;

		CullMode CullMode;
		WindingOrder WindingOrder;
		FillMode FillMode;
		PrimitiveTopology Topology;	
		bool PrimitiveRestart;

		InputLayout Layout;

		Rect2D Viewport;
		Rect2D Scissor;
		
		ShaderBinary VertexShader;
		ShaderBinary FragmentShader;
		ShaderBinary TesselationControlShader;
		ShaderBinary TesselationEvaluationShader;
		ShaderBinary GeometryShader;

		Descriptor *Descriptors;
		uint32_t DescriptorCount;

		uint32_t SampleCount;

		float LineWidth;
		float PointSize;

		float PolygonOffsetFactor;
		float PolygonOffsetUnits;

		uint32_t ElementSize;
	};

	class NativePipeline {
	public:
		static Ref<NativePipeline> Create(const Ref<NativeGraphicsContext> &context, const PipelineInfo &info);

	public:
		NativePipeline() { }
		virtual ~NativePipeline() { }

		virtual CullMode GetCullMode() const = 0;
		virtual WindingOrder GetWindingOrder() const = 0;
		virtual FillMode GetFillMode() const = 0;
		virtual PrimitiveTopology GetTopology() const = 0;
		virtual InputLayout GetLayout() const = 0;
		virtual Rect2D GetViewport() const = 0;
		virtual Rect2D GetScissor() const = 0;
		virtual float GetPolygonOffsetFactor() const = 0;
		virtual float GetPolygonOffsetUnits() const = 0;

		virtual uint32_t GetSampleCount() const = 0;
		virtual uint8_t GetOutputMask() const = 0;
		virtual float GetLineWidth() const = 0;
		virtual float GetPointSize() const = 0;
		virtual uint32_t GetStageFlags() const = 0;

		virtual size_t GetElementSize() const = 0;

		virtual const InputLayout &GetInputLayout() const = 0;

		virtual void SetUniformBuffer(uint32_t binding, const Ref<NativeBuffer> &uniformBuffer) = 0;
		virtual void SetCombinedImageSampler(uint32_t binding, const Ref<NativeTexture> &texture, const Ref<NativeSampler> &sampler) = 0;
	};

}