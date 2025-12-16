#pragma once

#include <VeCamera.hpp>
#include <VeGameObject.hpp>

struct PointLightEntity{
	vem::vec4	position{};
	vem::vec4	color{};
};

struct GlobalUbo{
	vem::mat4			projection{1.f};
	vem::mat4			view{1.f};
	vem::mat4			inverseView{1.f};
	vem::vec4			ambientLightColor{1.f, 1.f, 1.f, .02f};
	PointLightEntity	pointLights[MAX_LIGHTS];
	int					numLights;
	int					textureOn;
};

struct FrameInfo{
	int					frameIndex;
	float				frameTime;
	VkCommandBuffer		commandBuffer;
	VeCamera			&camera;
	VkDescriptorSet		globalDescriptorSet;
	VeGameObject::Map	&gameObject;
};
