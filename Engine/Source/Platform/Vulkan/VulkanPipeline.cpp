#include "VulkanPipeline.hpp"

namespace Arcane {

	VkFormat ToVulkanFormat(InputElementType type) {
		switch (type) {
			case InputElementType::Vector2f32: return VK_FORMAT_R32G32_SFLOAT;
			case InputElementType::Vector3f32: return VK_FORMAT_R32G32B32_SFLOAT;
			case InputElementType::Vector4f32: return VK_FORMAT_R32G32B32A32_SFLOAT;
			case InputElementType::Vector2i8: return VK_FORMAT_R8G8_SINT;
			case InputElementType::Vector3i8: return VK_FORMAT_R8G8B8_SINT;
			case InputElementType::Vector4i8: return VK_FORMAT_R8G8B8A8_SINT;
			case InputElementType::Vector2i16: return VK_FORMAT_R16G16_SINT;
			case InputElementType::Vector3i16: return VK_FORMAT_R16G16B16_SINT;
			case InputElementType::Vector4i16: return VK_FORMAT_R16G16B16A16_SINT;
			case InputElementType::Vector2i32: return VK_FORMAT_R32G32_SINT;
			case InputElementType::Vector3i32: return VK_FORMAT_R32G32B32_SINT;
			case InputElementType::Vector4i32: return VK_FORMAT_R32G32B32A32_SINT;
			case InputElementType::Vector2u8: return VK_FORMAT_R8G8_UINT;
			case InputElementType::Vector3u8: return VK_FORMAT_R8G8B8_UINT;
			case InputElementType::Vector4u8: return VK_FORMAT_R8G8B8A8_UINT;
			case InputElementType::Vector2u16: return VK_FORMAT_R16G16_UINT;
			case InputElementType::Vector3u16: return VK_FORMAT_R16G16B16_UINT;
			case InputElementType::Vector4u16: return VK_FORMAT_R16G16B16A16_UINT;
			case InputElementType::Vector2u32: return VK_FORMAT_R32G32_UINT;
			case InputElementType::Vector3u32: return VK_FORMAT_R32G32B32_UINT;
			case InputElementType::Vector4u32: return VK_FORMAT_R32G32B32A32_UINT;
			case InputElementType::Float32: return VK_FORMAT_R32_SFLOAT;
			case InputElementType::Int32: return VK_FORMAT_R32_SINT;
			case InputElementType::UInt32: return VK_FORMAT_R32_UINT;
			default: return VK_FORMAT_UNDEFINED;
		}
	}

	VkPrimitiveTopology ToVulkanPrimitiveTopology(PrimitiveTopology topology) {
		switch (topology) {
			case PrimitiveTopology::PointList: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case PrimitiveTopology::LineList: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case PrimitiveTopology::LineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case PrimitiveTopology::TriangleList: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case PrimitiveTopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			default: return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
		}
	}

	VkPolygonMode ToVulkanPolygonMode(FillMode fillMode) {
		switch (fillMode) {
			case FillMode::Solid: return VK_POLYGON_MODE_FILL;
			case FillMode::Wireframe: return VK_POLYGON_MODE_LINE;
			case FillMode::Points: return VK_POLYGON_MODE_POINT;
			default: return VK_POLYGON_MODE_MAX_ENUM;
		}
	} 

	VkCullModeFlags ToVulkanCullMode(CullMode cullMode) {
		switch (cullMode) {
			case CullMode::None: return VK_CULL_MODE_NONE;
			case CullMode::Front: return VK_CULL_MODE_FRONT_BIT;
			case CullMode::Back: return VK_CULL_MODE_BACK_BIT;
			case CullMode::FrontAndBack: return VK_CULL_MODE_FRONT_AND_BACK;
			default: return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
		}
	}

	VulkanPipeline::VulkanPipeline(const Ref<VulkanGraphicsContext> &context, const PipelineInfo &info) : mContext(context) {
		VkShaderModuleCreateInfo shaderModuleCreateInfos[2];
		shaderModuleCreateInfos[0].sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfos[0].pNext = nullptr;
		shaderModuleCreateInfos[0].flags = 0;
		shaderModuleCreateInfos[0].codeSize = info.VertexShader.Size;
		shaderModuleCreateInfos[0].pCode = reinterpret_cast<uint32_t*>(info.VertexShader.Data);

		shaderModuleCreateInfos[1].sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderModuleCreateInfos[1].pNext = nullptr;
		shaderModuleCreateInfos[1].flags = 0;
		shaderModuleCreateInfos[1].codeSize = info.FragmentShader.Size;
		shaderModuleCreateInfos[1].pCode = reinterpret_cast<uint32_t*>(info.FragmentShader.Data);

		VkShaderModule shaderModules[2];
		AR_VULKAN_CHECK_RESULT(vkCreateShaderModule(context->GetLogicalDevice(), &shaderModuleCreateInfos[0], nullptr, &shaderModules[0]));
		AR_VULKAN_CHECK_RESULT(vkCreateShaderModule(context->GetLogicalDevice(), &shaderModuleCreateInfos[1], nullptr, &shaderModules[1]));

		VkPipelineShaderStageCreateInfo shaderStages[2];
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].flags = 0;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = shaderModules[0];
		shaderStages[0].pName = "main";

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].flags = 0;
		shaderStages[1].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[1].module = shaderModules[0];
		shaderStages[1].pName = "main";

		VkVertexInputBindingDescription vertexInputBinding;
		vertexInputBinding.binding = 0;
		vertexInputBinding.stride = info.Layout.GetTotalSize();
		vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		VkVertexInputAttributeDescription *vertexAttributes = new VkVertexInputAttributeDescription[info.Layout.GetElements().size()];
		
		size_t offset = 0;
		uint32_t index = 0;
		
		for (size_t i = 0; i < info.Layout.GetElements().size(); i++) {
			const InputElement &element = info.Layout.GetElements()[i];
			
			for (uint32_t j = 0; j < element.Count; j++) {
				vertexAttributes[i].location = index++;
				vertexAttributes[i].binding = 0;
				vertexAttributes[i].format = ToVulkanFormat(element.Type);
				vertexAttributes[i].offset = offset;
			}

			offset += element.Count * GetInputElementSize(element);
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.pNext = nullptr;
		vertexInputInfo.flags = 0;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &vertexInputBinding;
		vertexInputInfo.vertexAttributeDescriptionCount = info.Layout.GetElements().size();
		vertexInputInfo.pVertexAttributeDescriptions = vertexAttributes;

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.pNext = nullptr;
		inputAssemblyInfo.flags = 0;
		inputAssemblyInfo.topology = ToVulkanPrimitiveTopology(info.Topology);
		inputAssemblyInfo.primitiveRestartEnable = info.PrimitiveRestart;

		VkPipelineTessellationStateCreateInfo tessellationStateInfo{};
		tessellationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		tessellationStateInfo.pNext = nullptr;
		tessellationStateInfo.flags = 0;
		tessellationStateInfo.patchControlPoints = 1;

		VkViewport viewport{};
		viewport.x = info.Viewport.Position.X;
		viewport.y = info.Viewport.Position.Y;
		viewport.width = info.Viewport.Size.X;
		viewport.height = info.Viewport.Size.Y;

		VkRect2D scissor{};
		scissor.offset.x = info.Scissor.Position.X;
		scissor.offset.y = info.Scissor.Position.Y;
		scissor.extent.width = info.Scissor.Size.X;
		scissor.extent.height = info.Scissor.Size.Y;

		VkPipelineViewportStateCreateInfo viewportStateInfo{};
		viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateInfo.pNext = nullptr;
		viewportStateInfo.flags = 0;
		viewportStateInfo.viewportCount = 1;
		viewportStateInfo.pViewports = &viewport;
		viewportStateInfo.scissorCount = 1;
		viewportStateInfo.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizationStateInfo{};
		rasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationStateInfo.pNext = nullptr;
		rasterizationStateInfo.flags = 0;
		rasterizationStateInfo.depthClampEnable = VK_FALSE;
		rasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizationStateInfo.polygonMode = ToVulkanPolygonMode(info.FillMode);
		rasterizationStateInfo.cullMode = ToVulkanCullMode(info.CullMode);
		rasterizationStateInfo.frontFace = (info.WindingOrder == WindingOrder::Clockwise) ? VK_FRONT_FACE_CLOCKWISE : VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationStateInfo.depthBiasEnable = VK_FALSE;
		rasterizationStateInfo.lineWidth = info.LineWidth;

		VkPipelineMultisampleStateCreateInfo multisampleStateInfo{};
		multisampleStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleStateInfo.pNext = nullptr;
		multisampleStateInfo.flags = 0;
		multisampleStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // TODO: support multiple samples
		multisampleStateInfo.sampleShadingEnable = VK_FALSE;

		VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo{};
		depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilStateInfo.pNext = nullptr;
		depthStencilStateInfo.flags = 0;
		depthStencilStateInfo.depthTestEnable = VK_TRUE;
		depthStencilStateInfo.depthWriteEnable = VK_TRUE;
		depthStencilStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
		depthStencilStateInfo.stencilTestEnable = VK_FALSE;
		depthStencilStateInfo.front = {};
		depthStencilStateInfo.back = {};
		depthStencilStateInfo.minDepthBounds = 0.0f;
		depthStencilStateInfo.maxDepthBounds = 1.0f;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		VkPipelineColorBlendStateCreateInfo colorBlendStateInfo{};
		colorBlendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendStateInfo.pNext = nullptr;
		colorBlendStateInfo.flags = 0;
		colorBlendStateInfo.logicOpEnable = VK_FALSE;
		colorBlendStateInfo.logicOp = VK_LOGIC_OP_COPY;
		colorBlendStateInfo.attachmentCount = 1;
		colorBlendStateInfo.pAttachments = &colorBlendAttachment;
		colorBlendStateInfo.blendConstants[0] = 0.0f;
		colorBlendStateInfo.blendConstants[1] = 0.0f;
		colorBlendStateInfo.blendConstants[2] = 0.0f;
		colorBlendStateInfo.blendConstants[3] = 0.0f;

		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.pNext = nullptr;
		dynamicStateInfo.flags = 0;
		dynamicStateInfo.dynamicStateCount = 0;
		dynamicStateInfo.pDynamicStates = nullptr;

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
		descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutInfo.pNext = nullptr;
		descriptorSetLayoutInfo.flags = 0;


		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pNext = nullptr;
		pipelineLayoutInfo.flags = 0;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.pNext = nullptr;
		pipelineInfo.flags = 0;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
		pipelineInfo.pTessellationState = &tessellationStateInfo;
		pipelineInfo.pViewportState = &viewportStateInfo;
		pipelineInfo.pRasterizationState = &rasterizationStateInfo;
		pipelineInfo.pMultisampleState = &multisampleStateInfo;
		pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
		pipelineInfo.pColorBlendState = &colorBlendStateInfo;
		pipelineInfo.pDynamicState = &dynamicStateInfo;


		vkCreateGraphicsPipelines(context->GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline);
	}

	VulkanPipeline::~VulkanPipeline() {
		// Cleanup Vulkan pipeline resources here
	}

	void VulkanPipeline::SetUniformBuffer(uint32_t binding, const Ref<NativeBuffer> &uniformBuffer) {
		// Set uniform buffer logic
	}

	void VulkanPipeline::SetCombinedImageSampler(uint32_t binding, const Ref<NativeTexture> &texture, const Ref<NativeSampler> &sampler) {
		// Set combined image sampler logic
	}

}