#include "VeGameObject.hpp"
#include "VeModel.hpp"
#include "vem.hpp"
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
	pipelineConfig.rasterizationInfo.cullMode = VK_CULL_MODE_FRONT_BIT;

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

#define ALTAS_WIDTH  (2048)
#define ALTAS_HEIGHT (2048)
#define CELL_SIZE (512)

#define CELL_RAW(x, y) { \
	static_cast<float>((x)) / static_cast<float>(ALTAS_WIDTH), \
	static_cast<float>((y)) / static_cast<float>(ALTAS_HEIGHT) \
}

#define CELL_TL(x,y) CELL_RAW((x)       * CELL_SIZE + 0, (y)       * CELL_SIZE + 0)
#define CELL_TR(x,y) CELL_RAW(((x) + 1) * CELL_SIZE - 0, (y)       * CELL_SIZE + 0)
#define CELL_BL(x,y) CELL_RAW((x)       * CELL_SIZE + 0, ((y) + 1) * CELL_SIZE - 0)
#define CELL_BR(x,y) CELL_RAW(((x) + 1) * CELL_SIZE - 0, ((y) + 1) * CELL_SIZE - 0)

static vem::vec2 makeUV(int x, int y){
	return vem::vec2(static_cast<float>(x) / 4.f, static_cast<float>(y) / 4.f);
}

VeGameObject SkyboxRender::createSkyboxObject(VeDevice& device, const std::string &texture_path)
{
	(void)(texture_path);
	
	static const vem::vec3 WHITE = {1.f, 0.f, 0.f};
	
	static const VeModel::Builder skyboxBuilder = {
		.vertices = {
			// RIGHT
			{{ 1, -1, -1}, WHITE, {-1,  0,  0}, CELL_TR(2, 1)}, // TR
			{{ 1,  1, -1}, WHITE, {-1,  0,  0}, CELL_BR(2, 1)}, // BR
			{{ 1,  1,  1}, WHITE, {-1,  0,  0}, CELL_BL(2, 1)}, // BL
			{{ 1, -1,  1}, WHITE, {-1,  0,  0}, CELL_TL(2, 1)}, // TL

			// LEFT
			{{-1, -1,  1}, WHITE, { 1,  0,  0}, CELL_TR(0, 1)}, // TR
			{{-1,  1,  1}, WHITE, { 1,  0,  0}, CELL_BR(0, 1)}, // BR
			{{-1,  1, -1}, WHITE, { 1,  0,  0}, CELL_BL(0, 1)}, // BL
			{{-1, -1, -1}, WHITE, { 1,  0,  0}, CELL_TL(0, 1)}, // TL

			// BOTTOM
			{{-1,  1, -1}, WHITE, { 0, -1,  0}, CELL_BL(1, 2)},
			{{-1,  1,  1}, WHITE, { 0, -1,  0}, CELL_TL(1, 2)},
			{{ 1,  1,  1}, WHITE, { 0, -1,  0}, CELL_TR(1, 2)},
			{{ 1,  1, -1}, WHITE, { 0, -1,  0}, CELL_BR(1, 2)},

			// TOP (flip Bottom -> top and Left -> rights)
			{{-1, -1,  1}, WHITE, { 0,  1,  0}, CELL_BL(1, 0)}, // TR
			{{-1, -1, -1}, WHITE, { 0,  1,  0}, CELL_TL(1, 0)}, // BR
			{{ 1, -1, -1}, WHITE, { 0,  1,  0}, CELL_TR(1, 0)}, // BL
			{{ 1, -1,  1}, WHITE, { 0,  1,  0}, CELL_BR(1, 0)}, // TL

			// FRONT
			{{ 1, -1,  1}, WHITE, { 0,  0, -1}, CELL_TR(1, 1)}, // TR
			{{ 1,  1,  1}, WHITE, { 0,  0, -1}, CELL_BR(1, 1)}, // BR
			{{-1,  1,  1}, WHITE, { 0,  0, -1}, CELL_BL(1, 1)}, // BL
			{{-1, -1,  1}, WHITE, { 0,  0, -1}, CELL_TL(1, 1)}, // TL

			// BACK
			{{-1, -1, -1}, WHITE, { 0,  0,  1}, CELL_TR(3, 1)}, // TR
			{{-1,  1, -1}, WHITE, { 0,  0,  1}, CELL_BR(3, 1)}, // BR
			{{ 1,  1, -1}, WHITE, { 0,  0,  1}, CELL_BL(3, 1)}, // BL
			{{ 1, -1, -1}, WHITE, { 0,  0,  1}, CELL_TL(3, 1)}, // TL
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
