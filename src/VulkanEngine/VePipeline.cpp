#include <VePipeline.hpp>

using namespace std;

vector<char>	VePipeline::readFile(const string &filepath){
	ifstream	file(filepath, ios::ate | ios::binary);

	if (!file.is_open())
		throw (runtime_error("failed to open file: " + filepath));

	size_t			fileSize = static_cast<size_t>(file.tellg());
	vector<char>	buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return (buffer);
}

void	VePipeline::createGraphicsPipeline(
	const string &vertFilepath,
	const string &fragFilepath,
	const PipelineConfigInfo &configInfo
){
	auto	vertCode = readFile(vertFilepath);
	auto	fragCode = readFile(fragFilepath);

	createShaderModule(vertCode, &_vertShaderModule);
	createShaderModule(fragCode, &_fragShaderModule);

	VkPipelineShaderStageCreateInfo	shaderStages[2];
	shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStages[0].module = _vertShaderModule;
	shaderStages[0].pName = "main";
	shaderStages[0].flags = 0;
	shaderStages[0].pNext = nullptr;
	shaderStages[0].pSpecializationInfo = nullptr;
	shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStages[1].module = _fragShaderModule;
	shaderStages[1].pName = "main";
	shaderStages[1].flags = 0;
	shaderStages[1].pNext = nullptr;
	shaderStages[1].pSpecializationInfo = nullptr;

	auto	&bindingDescriptions = configInfo.bindingDescription;
	auto	&attributeDescriptions = configInfo.attributeDescription;

	VkPipelineVertexInputStateCreateInfo	vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

	VkGraphicsPipelineCreateInfo	pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
	pipelineInfo.pViewportState = &configInfo.viewportInfo;
	pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
	pipelineInfo.pMultisampleState = &configInfo.multisampleInfo;
	pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
	pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
	pipelineInfo.pDynamicState = &configInfo.dynamicStateInfo;
	pipelineInfo.layout = configInfo.pipelineLayout;
	pipelineInfo.renderPass = configInfo.renderPass;
	pipelineInfo.subpass = configInfo.subpass;
	pipelineInfo.basePipelineIndex = -1;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(
		_veDevice.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline) != 0
	)
		throw (runtime_error("failed to create graphics pipelines"));
}

void	VePipeline::createShaderModule(const vector<char>& code, VkShaderModule *shaderModule){
	VkShaderModuleCreateInfo	createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
	if (vkCreateShaderModule(_veDevice.device(), &createInfo, nullptr, shaderModule) != 0)
		throw (runtime_error("failed to create shader module"));
}

VePipeline::VePipeline(
	VeDevice &device,
	const string &vertFilepath,
	const string &fragFilepath,
	const PipelineConfigInfo &configInfo
) : _veDevice(device){
	createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
}

VePipeline::~VePipeline(){
	vkDestroyShaderModule(_veDevice.device(), _vertShaderModule, nullptr);
	vkDestroyShaderModule(_veDevice.device(), _fragShaderModule, nullptr);
	vkDestroyPipeline(_veDevice.device(), _graphicsPipeline, nullptr);
}

void	VePipeline::bind(VkCommandBuffer commandBuffer){
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);
}

void	VePipeline::defaultPipelineConfigInfo(PipelineConfigInfo &config){
	config.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	config.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	config.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	config.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	config.viewportInfo.viewportCount = 1;
	config.viewportInfo.pViewports = nullptr;
	config.viewportInfo.scissorCount = 1;
	config.viewportInfo.pScissors = nullptr;

	config.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	config.rasterizationInfo.depthClampEnable = VK_FALSE;
	config.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	config.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	config.rasterizationInfo.lineWidth = 1.0f;
	config.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
	config.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	config.rasterizationInfo.depthBiasEnable = VK_FALSE;

	config.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	config.multisampleInfo.sampleShadingEnable = VK_FALSE;
	config.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	config.colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	config.colorBlendAttachment.blendEnable = VK_FALSE;

	config.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	config.colorBlendInfo.logicOpEnable = VK_FALSE;
	config.colorBlendInfo.attachmentCount = 1;
	config.colorBlendInfo.pAttachments = &config.colorBlendAttachment;

	config.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	config.depthStencilInfo.depthTestEnable = VK_TRUE;
	config.depthStencilInfo.depthWriteEnable = VK_TRUE;
	config.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	config.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	config.depthStencilInfo.stencilTestEnable = VK_FALSE;

	config.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	config.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	config.dynamicStateInfo.pDynamicStates = config.dynamicStateEnables.data();
	config.dynamicStateInfo.dynamicStateCount =
		static_cast<uint32_t>(config.dynamicStateEnables.size());
	config.dynamicStateInfo.flags = 0;

	config.bindingDescription = VeModel::Vertex::getBindingDescriptions();
	config.attributeDescription = VeModel::Vertex::getAttributeDescriptions();
}

void	VePipeline::enableAlphaBlending(PipelineConfigInfo &config){
	config.colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	config.colorBlendAttachment.blendEnable = VK_TRUE;
	config.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	config.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	config.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	config.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	config.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	config.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
}
