#include <SkyboxRender.hpp>
#include <SimpleRender.hpp>

void	SkyboxRender::createPipelineLayout(DSLayout gSetLayout){
	VkPushConstantRange	pushConstantRange{};
	pushConstantRange.stageFlags =
		VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PushConstantData);

	vector<DSLayout>	descriptorSetLayouts{gSetLayout};

	VkPipelineLayoutCreateInfo	layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.setLayoutCount = static_cast<uint>(descriptorSetLayouts.size());
	layoutInfo.pSetLayouts = descriptorSetLayouts.data();
	layoutInfo.pushConstantRangeCount = 1;
	layoutInfo.pPushConstantRanges = &pushConstantRange;

	if (vkCreatePipelineLayout(
		_veDevice.device(), &layoutInfo, nullptr, &_pipelineLayout
	) != 0)
		throw (runtime_error("failed to create skybox pipeline layout"));
}

void	SkyboxRender::createPipeline(VkRenderPass renderPass){
	PipelineConfigInfo	pipelineConfig{};
	VePipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = _pipelineLayout;

	_vePipeline = make_unique<VePipeline>(
		_veDevice,
		"spirv/skybox.vert.spv",
		"spirv/skybox.frag.spv",
		pipelineConfig
	);
}

SkyboxRender::SkyboxRender(VeDevice &d, VkRenderPass rp, DSLayout layout)
: _veDevice(d){
	createPipelineLayout(layout);
	createPipeline(rp);
}

SkyboxRender::~SkyboxRender(){
	vkDestroyPipelineLayout(_veDevice.device(), _pipelineLayout, nullptr);
}

void	SkyboxRender::renderSkybox(FrameInfo &frameInfo, VeGameObject & skybox){
	auto	&obj = skybox;
	if (obj._model == nullptr)
		return ;
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
