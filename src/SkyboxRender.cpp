#include "VeGameObject.hpp"
#include "VeModel.hpp"
#include <SkyboxRender.hpp>
#include <SimpleRender.hpp>
#include <memory>

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


VeGameObject SkyboxRender::createSkyboxObject(VeDevice& device, const std::string &texture_path)
{
	(void)(texture_path);
	
	static const vem::vec3 WHITE = {1.f, 1.f, 1.f};
	
	static const VeModel::Builder skyboxBuilder = {
		.vertices = {
			// +X (panorama_0)
			{{ 1, -1, -1}, WHITE, {-1,  0,  0}, {0.0f/3.0f, 1.0f/2.0f}},
			{{ 1,  1, -1}, WHITE, {-1,  0,  0}, {0.0f/3.0f, 1.0f}},
			{{ 1,  1,  1}, WHITE, {-1,  0,  0}, {1.0f/3.0f, 1.0f}},
			{{ 1, -1,  1}, WHITE, {-1,  0,  0}, {1.0f/3.0f, 1.0f/2.0f}},

			// -X (panorama_1)
			{{-1, -1,  1}, WHITE, { 1,  0,  0}, {1.0f/3.0f, 1.0f/2.0f}},
			{{-1,  1,  1}, WHITE, { 1,  0,  0}, {1.0f/3.0f, 1.0f}},
			{{-1,  1, -1}, WHITE, { 1,  0,  0}, {2.0f/3.0f, 1.0f}},
			{{-1, -1, -1}, WHITE, { 1,  0,  0}, {2.0f/3.0f, 1.0f/2.0f}},

			// +Y (panorama_2)
			{{-1,  1, -1}, WHITE, { 0, -1,  0}, {2.0f/3.0f, 1.0f/2.0f}},
			{{-1,  1,  1}, WHITE, { 0, -1,  0}, {2.0f/3.0f, 1.0f}},
			{{ 1,  1,  1}, WHITE, { 0, -1,  0}, {1.0f,      1.0f}},
			{{ 1,  1, -1}, WHITE, { 0, -1,  0}, {1.0f,      1.0f/2.0f}},

			// -Y (panorama_3)
			{{-1, -1,  1}, WHITE, { 0,  1,  0}, {0.0f/3.0f, 1.0f/2.0f}},
			{{-1, -1, -1}, WHITE, { 0,  1,  0}, {0.0f/3.0f, 0.0f}},
			{{ 1, -1, -1}, WHITE, { 0,  1,  0}, {1.0f/3.0f, 0.0f}},
			{{ 1, -1,  1}, WHITE, { 0,  1,  0}, {1.0f/3.0f, 1.0f/2.0f}},

			// +Z (panorama_4)
			{{ 1, -1,  1}, WHITE, { 0,  0, -1}, {1.0f/3.0f, 1.0f/2.0f}},
			{{ 1,  1,  1}, WHITE, { 0,  0, -1}, {1.0f/3.0f, 1.0f}},
			{{-1,  1,  1}, WHITE, { 0,  0, -1}, {2.0f/3.0f, 1.0f}},
			{{-1, -1,  1}, WHITE, { 0,  0, -1}, {2.0f/3.0f, 1.0f/2.0f}},

			// -Z (panorama_5)
			{{-1, -1, -1}, WHITE, { 0,  0,  1}, {2.0f/3.0f, 1.0f/2.0f}},
			{{-1,  1, -1}, WHITE, { 0,  0,  1}, {2.0f/3.0f, 1.0f}},
			{{ 1,  1, -1}, WHITE, { 0,  0,  1}, {1.0f,      1.0f}},
			{{ 1, -1, -1}, WHITE, { 0,  0,  1}, {1.0f,      1.0f/2.0f}},
		},

		.indices = {
			0,  1,  2,  2,  3,  0,        // +X
			4,  5,  6,  6,  7,  4,        // -X
			8,  9, 10, 10, 11,  8,        // +Y
		   12, 13, 14, 14, 15, 12,        // -Y
		   16, 17, 18, 18, 19, 16,        // +Z
		   20, 21, 22, 22, 23, 20         // -Z
		}
	};
	VeGameObject obj;

	obj._model = make_shared<VeModel>(device, skyboxBuilder, 10);
	obj._transform.scale = {128.f, 128.f, 128.f};
	obj._color = WHITE;

	return obj;
}
