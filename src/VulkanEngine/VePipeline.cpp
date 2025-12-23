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

	VkPipelineVertexInputStateCreateInfo	pvisci{};
	pvisci.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	pvisci.vertexAttributeDescriptionCount = static_cast<uint>(
		attributeDescriptions.size());
	pvisci.vertexBindingDescriptionCount = static_cast<uint>(
		bindingDescriptions.size());
	pvisci.pVertexAttributeDescriptions = attributeDescriptions.data();
	pvisci.pVertexBindingDescriptions = bindingDescriptions.data();

	VkGraphicsPipelineCreateInfo	gpci{};
	gpci.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	gpci.stageCount = 2;
	gpci.pStages = shaderStages;
	gpci.pVertexInputState = &pvisci;
	gpci.pInputAssemblyState = &configInfo.inputAssemblyInfo;
	gpci.pViewportState = &configInfo.viewportInfo;
	gpci.pRasterizationState = &configInfo.rasterizationInfo;
	gpci.pMultisampleState = &configInfo.multisampleInfo;
	gpci.pColorBlendState = &configInfo.colorBlendInfo;
	gpci.pDepthStencilState = &configInfo.depthStencilInfo;
	gpci.pDynamicState = &configInfo.dynamicStateInfo;
	gpci.layout = configInfo.pipelineLayout;
	gpci.renderPass = configInfo.renderPass;
	gpci.subpass = configInfo.subpass;
	gpci.basePipelineIndex = -1;
	gpci.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(
		_veDevice.device(), nullptr, 1, &gpci, nullptr, &_graphicsPipeline
	) != 0)
		throw (runtime_error("failed to create graphics pipelines"));
}

void	VePipeline::createShaderModule(
	const vector<char>& code, VkShaderModule *shaderModule
){
	VkShaderModuleCreateInfo	createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint*>(code.data());
	if (vkCreateShaderModule(
		_veDevice.device(), &createInfo, nullptr, shaderModule
	) != 0)
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

void	VePipeline::bind(VkCommandBuffer cb){
	vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);
}

void	VePipeline::defaultPipelineConfigInfo(PipelineConfigInfo &c){
	c.inputAssemblyInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	c.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	c.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	c.viewportInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	c.viewportInfo.viewportCount = 1;
	c.viewportInfo.pViewports = nullptr;
	c.viewportInfo.scissorCount = 1;
	c.viewportInfo.pScissors = nullptr;

	c.rasterizationInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	c.rasterizationInfo.depthClampEnable = VK_FALSE;
	c.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	c.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	c.rasterizationInfo.lineWidth = 1.0f;
	c.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
	c.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	c.rasterizationInfo.depthBiasEnable = VK_FALSE;

	c.multisampleInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	c.multisampleInfo.sampleShadingEnable = VK_FALSE;
	c.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	c.colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	c.colorBlendAttachment.blendEnable = VK_FALSE;

	c.colorBlendInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	c.colorBlendInfo.logicOpEnable = VK_FALSE;
	c.colorBlendInfo.attachmentCount = 1;
	c.colorBlendInfo.pAttachments = &c.colorBlendAttachment;

	c.depthStencilInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	c.depthStencilInfo.depthTestEnable = VK_TRUE;
	c.depthStencilInfo.depthWriteEnable = VK_TRUE;
	c.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	c.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	c.depthStencilInfo.stencilTestEnable = VK_FALSE;

	c.dynamicStateEnables =
		{VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
	c.dynamicStateInfo.sType =
		VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	c.dynamicStateInfo.pDynamicStates = c.dynamicStateEnables.data();
	c.dynamicStateInfo.dynamicStateCount =
		static_cast<uint>(c.dynamicStateEnables.size());
	c.dynamicStateInfo.flags = 0;

	c.bindingDescription = VeModel::Vertex::getBindingDescriptions();
	c.attributeDescription = VeModel::Vertex::getAttributeDescriptions();
}

void	VePipeline::enableAlphaBlending(PipelineConfigInfo &c){
	c.colorBlendAttachment.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	c.colorBlendAttachment.blendEnable = VK_TRUE;
	c.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	c.colorBlendAttachment.dstColorBlendFactor =
		VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	c.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	c.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	c.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	c.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
}
