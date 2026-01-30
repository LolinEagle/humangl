#version 450

layout(binding = 1) uniform sampler2D texSampler;
struct PointLight {
	vec4	position;
	vec4	color;
};

struct TexIdMapping {
	vec2	tl;
	vec2	br;
	//uint	flags;
};


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragPosWorld;
layout(location = 2) in vec3 fragNormalWorld;
layout(location = 3) in vec2 fragUv;
layout(location = 4) in flat uint texId;
layout(location = 0) out vec4 outColor;
layout(location = 4) out uint outTexId;

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4			projection;
	mat4			view;
	mat4			invView;
	vec4			ambientLightColor;
	PointLight		pointLights[16];
	int				numLights;
	int				textureOn;
} ubo;

layout(set = 0, binding = 2) uniform TexIdMap {
	TexIdMapping texUv[128];
} texIdMap;

void	main(void){
	// here do the flipping, rotating and all;
	outTexId = texId;

	TexIdMapping mapping = texIdMap.texUv[texId];
	vec2 mappedUv = mix(mapping.tl, mapping.br, fragUv);
	outColor = texture(texSampler, mappedUv);
}
