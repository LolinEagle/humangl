#pragma once

#include <VeCamera.hpp>
#include <VeGameObject.hpp>

struct PointLightEntity{
	vem::vec4	position{};
	vem::vec4	color{};
};

// this is an enum that should be used as a bitfield
// meaning that you musn't assume that a value is only a single enum variant
// it *will* contain more than one variant
// use `(val & VARIANT) != 0` to check for variant
enum TexIdMappingTransform{
	TMAP_NONE 				= 0,
	TMAP_FLIP_HORIZONTAL	= 1 << 0,
	TMAP_FLIP_VERTICAL		= 1 << 1,
	TMAP_ROT_90				= 1 << 2,
	TMAP_ROT_180			= 1 << 3,
	TMAP_ROT_270			= 1 << 4,
};

// this represent a single texture -> mapped from a id to an actual texture
// it is to be used with a texture atlas
struct TexIdMapping{
	vem::vec2				topLeftUv		= {0.f, 0.f};
	vem::vec2				bottomLeftUv	= {1.f, 1.f};
	//uint					transform		= TMAP_NONE;
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

struct TexIdData {
	TexIdMapping		texUv[MAX_TEX_ID] = {};
};

struct FrameInfo{
	int					frameIndex;
	float				frameTime;
	VkCommandBuffer		commandBuffer;
	VeCamera			&camera;
	VkDescriptorSet		globalDescriptorSet;
	VeGameObject::Map	&gameObject;
};
