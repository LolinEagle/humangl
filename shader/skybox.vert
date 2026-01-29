#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;
layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragPosWorld;
layout(location = 2) out vec3 fragNormalWorld;
layout(location = 3) out vec2 fragUv;

struct PointLight {
	vec4	position;
	vec4	color;
};

layout(set = 0, binding = 0) uniform GlobalUbo {
	mat4		projection;
	mat4		view;
	mat4		invView;
	vec4		ambientLightColor;
	PointLight	pointLights[16];
	int			numLights;
	int			textureOn;
	vec2		texUv[32];
} ubo;

layout(push_constant) uniform Push {
	mat4	modelMatrix;
	mat4	normalMatrix;
} push;

void	main(void){
	fragUv = uv;
	// trim the last column (?) of the matrix -> removed rotation data
	mat4 viewNoRot = mat4(mat3(ubo.view));

	// Transforms position into world space coordinates
	vec4	positionWorld = push.modelMatrix * vec4(position, 1.0);

	vec4 pos = ubo.projection * ubo.view * positionWorld;

	gl_Position = pos.xyww;
	gl_Position.z *= 0.999;
}
