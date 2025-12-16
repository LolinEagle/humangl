#include <SimpleRender.hpp>

void	SimpleRender::createPipelineLayout(VkDescriptorSetLayout globalSetLayout){
	VkPushConstantRange	pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PushConstantData);

	vector<VkDescriptorSetLayout>	descriptorSetLayouts{globalSetLayout};

	VkPipelineLayoutCreateInfo	layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	layoutInfo.pSetLayouts = descriptorSetLayouts.data();
	layoutInfo.pushConstantRangeCount = 1;
	layoutInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(_veDevice.device(), &layoutInfo, nullptr, &_pipelineLayout) != 0)
		throw (runtime_error("failed to create pipeline layout"));
}

void	SimpleRender::createPipeline(VkRenderPass renderPass){
	PipelineConfigInfo	pipelineConfig{};
	VePipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = _pipelineLayout;

	_vePipeline = make_unique<VePipeline>(
		_veDevice,
		"shader/simpleShader.vert.spv",
		"shader/simpleShader.frag.spv",
		pipelineConfig
	);
}

SimpleRender::SimpleRender(VeDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout layout)
: _veDevice(device){
	createPipelineLayout(layout);
	createPipeline(renderPass);
}

SimpleRender::~SimpleRender(){
	vkDestroyPipelineLayout(_veDevice.device(), _pipelineLayout, nullptr);
}

void	SimpleRender::renderObjects(FrameInfo &frameInfo){
	_vePipeline->bind(frameInfo.commandBuffer);
	vkCmdBindDescriptorSets(
		frameInfo.commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		_pipelineLayout,
		0,
		1,
		&frameInfo.globalDescriptorSet,
		0,
		nullptr
	);
	for (auto &kv: frameInfo.gameObject){
		auto	&obj = kv.second;
		if (obj._model == nullptr)
			continue ;

		PushConstantData	push{};
		push.modelMatrix = obj._transform.mat4();
		push.normalMatrix = obj._transform.normalMatrix();

		vkCmdPushConstants(
			frameInfo.commandBuffer,
			_pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(PushConstantData),
			&push
		);
		obj._model->bind(frameInfo.commandBuffer);
		obj._model->draw(frameInfo.commandBuffer);
	}
}
