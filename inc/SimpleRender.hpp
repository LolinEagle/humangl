#pragma once

#include <VeCamera.hpp>
#include <VeDevice.hpp>
#include <VeFrameInfo.hpp>
#include <VeGameObject.hpp>
#include <VePipeline.hpp>

using namespace std;
using DSLayout = VkDescriptorSetLayout;

struct PushConstantData{
	vem::mat4	modelMatrix{1.f};
	vem::mat4	normalMatrix{1.f};
};

class SimpleRender{
private:
	VeDevice				&_veDevice;
	unique_ptr<VePipeline>	_vePipeline;
	VkPipelineLayout		_pipelineLayout;

	void	createPipelineLayout(DSLayout globalSetLayout);
	void	createPipeline(VkRenderPass renderPass);
public:
	SimpleRender(VeDevice &d, VkRenderPass rp, DSLayout layout);
	~SimpleRender();

	void	renderObjects(FrameInfo &frameInfo);
};
