#pragma once

#include <VeCamera.hpp>
#include <VeDevice.hpp>
#include <VeFrameInfo.hpp>
#include <VeGameObject.hpp>
#include <VePipeline.hpp>

using namespace std;

struct PushConstantData{
	vem::mat4	modelMatrix{1.f};
	vem::mat4	normalMatrix{1.f};
};

class SimpleRender{
	private:
		VeDevice				&_veDevice;
		unique_ptr<VePipeline>	_vePipeline;
		VkPipelineLayout		_pipelineLayout;

		void	createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void	createPipeline(VkRenderPass renderPass);
	public:
		SimpleRender(VeDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout layout);
		~SimpleRender();

		void	renderObjects(FrameInfo &frameInfo);
};
