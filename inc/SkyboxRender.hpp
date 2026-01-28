#pragma once

#include <VeCamera.hpp>
#include <VeDevice.hpp>
#include <VeFrameInfo.hpp>
#include <VeGameObject.hpp>
#include <VePipeline.hpp>

using namespace std;
using DSLayout = VkDescriptorSetLayout;

class SkyboxRender{
private:
	VeDevice				&_veDevice;
	unique_ptr<VePipeline>	_vePipeline;
	VkPipelineLayout		_pipelineLayout;

	void	createPipelineLayout(DSLayout globalSetLayout);
	void	createPipeline(VkRenderPass renderPass);
public:
	SkyboxRender(VeDevice &d, VkRenderPass rp, DSLayout layout);
	~SkyboxRender();

	void	renderSkybox(FrameInfo &frameInfo, VeGameObject &skybox);

	static VeGameObject createSkyboxObject(VeDevice& device, const std::string &texture_path);
};
