#include "VeGameObject.hpp"
#include "VeModel.hpp"
#include "vem.hpp"
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

// corner = 0 (topleft)
// corner = 1 (bottomleft)
// corner = 2 (bottomright)
// corner = 3 (topright)
static vem::vec2 makeUV(int x, int y, int corner){
	if (corner == 1) y++;
	if (corner == 2) {
		x++;
		y++;
	}
	if (corner == 3) x++;

	return vem::vec2(static_cast<float>(x) / 4.f, static_cast<float>(y) / 4.f);
}

VeGameObject SkyboxRender::createSkyboxObject(VeDevice& device, const std::string &texture_path)
{
	(void)(texture_path);
	
	static const vem::vec3 WHITE = {1.f, 0.f, 0.f};
	
	static const VeModel::Builder skyboxBuilder = {
		.vertices = {
			// RIGHT
			{{ 1, -1, -1}, WHITE, {-1,  0,  0}, makeUV(0, 1, 3)}, // TR
			{{ 1,  1, -1}, WHITE, {-1,  0,  0}, makeUV(0, 1, 2)}, // BR
			{{ 1,  1,  1}, WHITE, {-1,  0,  0}, makeUV(0, 1, 1)}, // BL
			{{ 1, -1,  1}, WHITE, {-1,  0,  0}, makeUV(0, 1, 0)}, // TL

			// LEFT
			{{-1, -1,  1}, WHITE, { 1,  0,  0}, makeUV(1, 1, 3)}, // TR
			{{-1,  1,  1}, WHITE, { 1,  0,  0}, makeUV(1, 1, 2)}, // BR
			{{-1,  1, -1}, WHITE, { 1,  0,  0}, makeUV(1, 1, 1)}, // BL
			{{-1, -1, -1}, WHITE, { 1,  0,  0}, makeUV(1, 1, 0)}, // TL

			// BOTTOM
			{{-1,  1, -1}, WHITE, { 0, -1,  0}, makeUV(1, 1, 3)},
			{{-1,  1,  1}, WHITE, { 0, -1,  0}, makeUV(1, 1, 2)},
			{{ 1,  1,  1}, WHITE, { 0, -1,  0}, makeUV(1, 1, 1)},
			{{ 1,  1, -1}, WHITE, { 0, -1,  0}, makeUV(1, 1, 0)},

			// TOP
			{{-1, -1,  1}, WHITE, { 0,  1,  0}, makeUV(1, 1, 3)},
			{{-1, -1, -1}, WHITE, { 0,  1,  0}, makeUV(1, 1, 2)},
			{{ 1, -1, -1}, WHITE, { 0,  1,  0}, makeUV(1, 1, 1)},
			{{ 1, -1,  1}, WHITE, { 0,  1,  0}, makeUV(1, 1, 0)},

			// FRONT
			{{ 1, -1,  1}, WHITE, { 0,  0, -1}, makeUV(1, 1, 3)}, // TR
			{{ 1,  1,  1}, WHITE, { 0,  0, -1}, makeUV(1, 1, 2)}, // BR
			{{-1,  1,  1}, WHITE, { 0,  0, -1}, makeUV(1, 1, 1)}, // BL
			{{-1, -1,  1}, WHITE, { 0,  0, -1}, makeUV(1, 1, 0)}, // TL

			// BACK
			{{-1, -1, -1}, WHITE, { 0,  0,  1}, makeUV(1, 1, 3)}, // TR
			{{-1,  1, -1}, WHITE, { 0,  0,  1}, makeUV(1, 1, 2)}, // BR
			{{ 1,  1, -1}, WHITE, { 0,  0,  1}, makeUV(1, 1, 1)}, // BL
			{{ 1, -1, -1}, WHITE, { 0,  0,  1}, makeUV(1, 1, 0)}, // TL
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
