#include <PointLight.hpp>

void	PointLight::createPipelineLayout(VkDescriptorSetLayout globalSetLayout){
	VkPushConstantRange	pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(PointLightPushConstants);

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

void	PointLight::createPipeline(VkRenderPass renderPass){
	PipelineConfigInfo	pipelineConfig{};
	VePipeline::defaultPipelineConfigInfo(pipelineConfig);
	VePipeline::enableAlphaBlending(pipelineConfig);
	pipelineConfig.attributeDescription.clear();
	pipelineConfig.bindingDescription.clear();
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = _pipelineLayout;

	_vePipeline = make_unique<VePipeline>(
		_veDevice,
		"shader/pointLight.vert.spv",
		"shader/pointLight.frag.spv",
		pipelineConfig
	);
}

PointLight::PointLight(VeDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout layout)
: _veDevice(device){
	createPipelineLayout(layout);
	createPipeline(renderPass);
}

PointLight::~PointLight(){
	vkDestroyPipelineLayout(_veDevice.device(), _pipelineLayout, nullptr);
}

void	PointLight::update(FrameInfo &frameInfo, GlobalUbo &ubo){
	auto	rotateLight = vem::rotate(vem::mat4(1.f), frameInfo.frameTime, {0.f, -1.f, 0.f});
	int		lightIndex = 0;

	for (auto &kv: frameInfo.gameObject){
		auto	&obj = kv.second;
		if (obj._pointLight == nullptr)
			continue ;

		// Time
		obj._transform.translation = vem::vec3(
			rotateLight * vem::vec4(obj._transform.translation, 1.f)
		);
		ubo.pointLights[lightIndex].position = vem::vec4(obj._transform.translation, 1.f);
		ubo.pointLights[lightIndex].color = vem::vec4(obj._color, obj._pointLight->lightIntensity);
		lightIndex++;
	}
	ubo.numLights = lightIndex;
}

void	PointLight::render(FrameInfo &frameInfo){
	map<float, uint32_t>	sorted;

	for (auto &kv: frameInfo.gameObject){
		auto	&obj = kv.second;
		if (obj._pointLight == nullptr)
			continue ;

		auto	offset = frameInfo.camera.getPosition() - obj._transform.translation;
		float	disSquared = vem::dot(offset, offset);

		sorted[disSquared] = obj.getId();
	}
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
	// Iterate through sorted lights in reverse order
	for (auto it = sorted.rbegin(); it != sorted.rend(); it++){
		auto	&obj = frameInfo.gameObject.at(it->second);// Use obj id to find light object

		PointLightPushConstants	push{};
		push.position = vem::vec4(obj._transform.translation, 1.f);
		push.color = vem::vec4(obj._color, obj._pointLight->lightIntensity);
		push.radius = obj._transform.scale.x;
		vkCmdPushConstants(
			frameInfo.commandBuffer,
			_pipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			0,
			sizeof(PointLightPushConstants),
			&push
		);
		vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
	}
}
